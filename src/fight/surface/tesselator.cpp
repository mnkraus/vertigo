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

#include "tesselator.h"
#include "betaspline.h"
#include "element.h"
#include "surface.h"


namespace fight {


Tesselator defaultTesselator(3, NULL);

Tesselator* Tesselator::get(Surface *surface)
{
    defaultTesselator.m_surface = surface;
    return &defaultTesselator;
}


Tesselator::Tesselator(int MaxLevel, Surface *surface) :
    m_surface(surface)
{
    m_splines << NULL;
    m_splines << NULL;
    for (int i = 2; i <= MaxLevel; i++)
        m_splines << new BetaSpline(i, 1, 0);
    InitIndices(MaxLevel);
}


Tesselator::~Tesselator()
{
    qDeleteAll(m_splines);
    delete indices;
}


void Tesselator::InitIndices(int MaxLevel)
{
	int i, j, k, l, t;
	int Level;

	indices = new short[4*(MaxLevel - 1)*(MaxLevel - 1)*3];

	k = 0;
	i = 0;
	j = 1;
	Level = 2;
	while (Level <= MaxLevel)
	{
		int i0 = i;
		int j0 = j;
		for (t = 0; t < 4; t++)
		{
			for (l = 0; l < Level - 2; l++)
			{
				indices[k++] = (short)i;
				indices[k++] = (short)j;
				indices[k++] = (short)(j + 1);

				indices[k++] = (short)i;
				j++;
				indices[k++] = (short)j;
				i = (i - i0 + 1)%(4*Level - 8) + i0;
				indices[k++] = (short)i;
			}
			indices[k++] = (short)i;
			indices[k++] = (short)j;
			indices[k++] = (short)((j - j0 + 1)%(4*Level - 4) + j0);
			j++;
		}
		i = j0;
		Level++;
	}
}


void Tesselator::PrepareElementSubset(int level, QVector3D scale, int x, int y, int textureId, QVector2D t0, QVector2D tu, QVector2D tv, Element *element)
{
	int i, j, k, l;

	k = element->numVertices(textureId);

    BetaSpline *spline = m_splines[level];
	spline->InitFrame(&height, x, y);
    QVector3D trans(x, y, 0);

	float dt = 1.0f/(2*(level - 1));
	int u = level - 1;
	int v = level - 1;

	QVector3D o = spline->Beta_3_3(u, v);
	QVector3D t, b;
	spline->Beta_TB(u, v, &t, &b);
    element->addVertex(textureId, (o + trans)*scale, QVector3D::crossProduct(t, b), t0 + tu*u*dt + tv*v*dt);
    float z = o.z();

	l = 2;
	while (l <= level)
	{
		u -= 1;
		v -= 1;
		int du = 2;
		int dv = 0;
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < l - 1; i++, u += du, v += dv)
			{
				o = spline->Beta_3_3(u, v);
				spline->Beta_TB(u, v, &t, &b);
                element->addVertex(textureId, (o + trans)*scale, QVector3D::crossProduct(t, b), t0 + tu*u*dt + tv*v*dt);
			}
			int tmp = du;
			du = -dv;
			dv = tmp;
		}
		l++;
	}

	for (i = 0; i < 4*(level - 1)*(level - 1); i++)
        element->addTriangle(textureId, k + indices[i*3 + 0], k + indices[i*3 + 2], k + indices[i*3 + 1]);
}


float Tesselator::heightAt(QVector2D pos) const
{
    const int x = pos.x();
    const int y = pos.y();
    const float dx = pos.x() - x;
    const float dy = pos.y() - y;

    BetaSpline *spline = m_splines[2];
	spline->InitFrame(&height, x, y);

    return spline->Beta_3_3(dx, dy).z();
}


float Tesselator::height(int x, int y)
{
    return defaultTesselator.m_surface->height(x, y);
}


Element* Tesselator::tesselate(QRect rect, int level, QVector3D scale, QByteArray &textureMap, QByteArray &textureDir, int mapping)
{
    Element* element = new Element(m_surface, rect);

    for (int y = rect.y(); y <= rect.bottom(); y++) {
        for (int x = rect.x(); x <= rect.right(); x++) {
            const int x1 = ((y & 0xff) << 8) + (x & 0xff);

            const quint8 m = textureMap[x1];
            const quint8 d = textureDir[x1];

            int texture = (m >> 2);

            switch (mapping) {
            case 0:
                if (texture == 9)
                    texture = 15;
                break;
            case 1:
                if (texture == 9)
                    texture = 15;
                else if (texture == 10)
                    texture = 16;
                break;
            case 2:
                if (texture == 9)
                    texture = 15;
                else if (texture == 11)
                    texture = 14;
                break;
            }

            const float subTextureX = (m & 0x01) ? 0.5f : 0.0f;
            const float subTextureY = (m & 0x02) ? 0.5f : 0.0f;
            int rotate = -2 + (d & 0x03);
            if (rotate < 0)
                rotate += 4;
            const bool flip = !(d & 0x04);

            const float margin = 0.002f;
            QList<QVector2D> texCoords;
            texCoords << QVector2D(0.5f + subTextureX - margin, 0.0f + subTextureY + margin);
            texCoords << QVector2D(0.0f + subTextureX + margin, 0.0f + subTextureY + margin);
            texCoords << QVector2D(0.0f + subTextureX + margin, 0.5f + subTextureY - margin);
            texCoords << QVector2D(0.5f + subTextureX - margin, 0.5f + subTextureY - margin);

            if (flip) {
                qSwap(texCoords[0], texCoords[1]);
                qSwap(texCoords[2], texCoords[3]);
            }

            for (; rotate > 0; rotate--)
                texCoords.append(texCoords.takeFirst());

            PrepareElementSubset(level, scale, x, y, texture, texCoords[1], texCoords[0] - texCoords[1], texCoords[2] - texCoords[1], element);
        }
    }

    return element;
}


} // namespace fight
