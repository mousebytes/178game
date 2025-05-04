#include "_fonts.h"

_fonts::_fonts()
{
    //ctor
    framesX=framesY=1;
    pos = {0, 0, -2};
    scale = {0.2, 0.2};

    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;
}

_fonts::~_fonts()
{
    //dtor
}

void _fonts::initFonts(const char* fileName, int fx, int fy)
{
    framesX = fx;
    framesY = fy;

    xMin = 0;
    xMax = 1.0 / (float)framesX;
    yMax = 1.0 / (float)framesY;
    yMin = yMax - (1.0 / (float)framesY);

    tex->loadTexture(fileName);
}

void _fonts::setPosition(float x, float y, float z)
{
    pos = {x, y, z};
}

void _fonts::setSize(float sx, float sy)
{
    scale = {sx, sy};
}

void _fonts::drawText(string text)
{
    tex->textureBinder();
    glColor3f(1, 1, 1);

    float charWidth = 1.0f / (float)framesX;
    float charHeight = 1.0f / (float)framesY;

    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];

        int fx = 0, fy = 0;
        if(!getFontFrame(c,fx,fy)) continue;

        float xMinChar = fx * charWidth;
        float xMaxChar = xMinChar + charWidth;
        float yMaxChar = 1.0f - (fy * charHeight);
        float yMinChar = yMaxChar - charHeight;

        glPushMatrix();
        glTranslatef(pos.x + (i * scale.x * 1.5f), pos.y, pos.z);
        glScalef(scale.x, scale.y, 1.0f);

        glBegin(GL_QUADS);
            glTexCoord2f(xMinChar, yMaxChar); glVertex3f(vert[0].x, vert[0].y, vert[0].z);
            glTexCoord2f(xMaxChar, yMaxChar); glVertex3f(vert[1].x, vert[1].y, vert[1].z);
            glTexCoord2f(xMaxChar, yMinChar); glVertex3f(vert[2].x, vert[2].y, vert[2].z);
            glTexCoord2f(xMinChar, yMinChar); glVertex3f(vert[3].x, vert[3].y, vert[3].z);
        glEnd();

        glPopMatrix();
    }
}

bool _fonts::getFontFrame(char c, int& fx, int& fy)
{
    if (c >= 'a' && c <= 'z') c -= 32; // normalize lowercase to uppercase

    if(c >= '0' && c <='9')
    {
        fx = 1 + (c-'0');
        fy = 6;
    }
    if(c>= 'A' && c<='L')
    {
        fx = 3 + (c-'A');
        fy = 5;
    }
    if(c>='M' && c<='Z')
    {
        fx = (c-'M');
        fy = 4;
    }

    if(c==' ')
    {
        fx = 0;
        fy = 7;
    }

    //clamp
    if (fx < 0 || fy < 0 || fx >= framesX || fy >= framesY)
        return false;

    return true;
}
