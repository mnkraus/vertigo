/***************************************************************************
 *  Copyright (C) 2010  Philipp Nordhus                                    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "video.h"
#include "util/rle.h"
#include <QDataStream>


namespace gfx {


Video::Video()
{

}


Video::Video(const QString &filename)
{
    open(filename);
}


void Video::open(const QString &filename)
{
    m_file.close();
    m_videoPos = 0;
    m_entries.clear();
    m_frame.clear();

    m_file.setFileName(filename);
    m_file.open(QFile::ReadOnly);

    QDataStream stream(&m_file);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 totalFrames;
    quint32 audioFrames;
    quint32 videoFrames;
    quint32 framerate;
    quint32 hasAudio;
    quint32 channels;
    quint32 sampleRate;

    stream.skipRawData(8);
    stream >> m_width >> m_height;
    stream >> totalFrames >> audioFrames >> videoFrames;
    stream >> framerate;
    stream >> hasAudio >> channels >> sampleRate;
    stream.skipRawData(4);

    const quint32 dataOffset = ((13 * sizeof(quint32) + totalFrames * 4 * sizeof(quint32) - 1) / 2048 + 1) * 2048;

    quint32 total = 0;
    for (unsigned int i = 0; i < totalFrames; i++) {
        quint32 type;
        quint32 unknown;
        quint32 length;
        quint32 offset;
        stream >> type >> unknown >> length >> offset;

        if (offset == 0)
            offset = dataOffset + total;
        else
            offset += dataOffset;

        Entry entry;
        entry.length = length;
        entry.offset = offset;
        entry.type = Entry::Type(type);

        m_entries.append(entry);

        total += length;
    }

    m_time.start();
}


QImage Video::getFrame()
{
    bool gotFrame = false;

    if (atEnd())
        return QImage();

    if (m_time.elapsed() < 100)
        return QImage();

    m_time.restart();

    while (!atEnd() && !gotFrame) {
        const Entry &entry = m_entries[m_videoPos++];

        m_file.seek(entry.offset);

        switch (entry.type) {
        case Entry::ColorTable:
            loadColorTable(m_file.read(entry.length));
            break;

        case Entry::VideoFull:
            loadVideoFull(m_file.read(entry.length));
            gotFrame = true;
            break;

        case Entry::VideoDiff:
            loadVideoDiff(m_file.read(entry.length));
            gotFrame = true;
            break;
        }
    }

    if (gotFrame)
        return createImage();

    return QImage();
}


QByteArray Video::getAudio()
{

}


bool Video::atEnd() const
{
    return m_videoPos >= m_entries.size();
}


void Video::loadColorTable(const QByteArray &data)
{
    m_colorTable.load(data.left(256 * 3));
    if (data.size() > 256 * 3)
        m_indexMap = data.right(data.size() - 256 * 3);
}


void Video::loadVideoFull(const QByteArray &data)
{
    util::RLE rle;
    m_frame = rle.decode(data);
}


void Video::loadVideoDiff(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 sizeIndices;
    quint32 sizeIndicesUnpacked;
    quint32 sizeBitmap;
    quint32 sizeBitmapUnpacked;
    stream >> sizeIndices >> sizeIndicesUnpacked >> sizeBitmap >> sizeBitmapUnpacked;

    QByteArray indices = QByteArray::fromRawData(data.data() + 4 * sizeof(quint32), sizeIndices);
    QByteArray bitmap = QByteArray::fromRawData(data.data() + 4 * sizeof(quint32) + sizeIndices, sizeBitmap);

    util::RLE rle;
    if (sizeIndices != sizeIndicesUnpacked)
        indices = rle.decode(indices);
    if (sizeBitmap != sizeBitmapUnpacked)
        bitmap = rle.decode(bitmap);

    quint32 num = 0;
    quint32 pos = 0;
    for (unsigned int i = 0; i < sizeBitmapUnpacked; i++) {
        for (int j = 7; j >= 0; j--) {
            if ((bitmap[i] >> j) & 0x01) {
                m_frame[pos] = indices[num];
                num++;
            }
            pos++;
        }
    }
}


QImage Video::createImage()
{
    QImage image(m_width, m_height, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(m_colorTable);

    const bool wide = (m_width * m_height) != quint32(m_frame.size());

    quint32 pos = 0;
    for (unsigned int y = 0; y < m_height; y++) {
        quint8 *line = image.scanLine(y);

        if (wide) {
            if (y % 2 == 1)
                *line++ = m_frame[pos];

            for (unsigned int x = y % 2; x < m_width; x += 2) {
                *line++ = m_frame[pos];
                *line++ = m_indexMap[quint8(m_frame[pos]) * 256 + quint8(m_frame[pos + 1])];
                pos++;
            }

            if (y % 2 == 0)
                *line++ = m_frame[pos - 1];
        } else {
            qMemCopy(line, m_frame, m_width);
        }
    }

    return image;
}


} // namespace gfx
