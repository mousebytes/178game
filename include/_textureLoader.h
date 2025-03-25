#ifndef _TEXTURELOADER_H
#define _TEXTURELOADER_H

#include<_common.h>
#include<SOIL.h>

class _textureLoader
{
    public:
        _textureLoader();
        virtual ~_textureLoader();

        int width,height;   // keep image width & height
        unsigned char* image; // read image to temp location
        GLuint tex;         // Texture buffer handler

        void loadTexture(char *);
        void textureBinder();

    protected:

    private:
};

#endif // _TEXTURELOADER_H
