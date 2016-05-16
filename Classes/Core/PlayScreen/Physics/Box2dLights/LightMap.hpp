//
// Created by Priyanshu Singh on 10/05/16.
//

#ifndef PLATFORMER_LIGHTMAP_HPP
#define PLATFORMER_LIGHTMAP_HPP


USING_NS_CC;



namespace box2dLight {

    class LightHandler;

    class LightMap : public cocos2d::Sprite {


    private:
        int _textureWidth;
        int _textureHeight;

        LightHandler *_lightHandler;

        GLProgram *_shadowShader;
        GLProgram *_diffuseShader;
        GLProgram *_blurShader;
        GLProgram *_withoutShadowShader;

        bool lightMapDrawingDisabled;

        Mat4 viewProjectionMatrix;

        RenderTexture *mainCanvas;
        CustomCommand _preBlurCommand;
        CustomCommand _postBlurCommand;

        Sprite *sprite;
        Texture2D *texture2D;

        TrianglesCommand _blur1,_blur2;


        void pushTriangleCommand(Renderer *renderer, const Mat4 &transform, uint32_t flags, TrianglesCommand *cmd);

        void onPreBlurDraw(const Mat4 &transform, uint32_t flags, Sprite **sprite, Texture2D **texture);
        void onPostBlurDraw(const Mat4 &transform, uint32_t flags);


    public:

        static const char *ATTRIBUTE_NAME_FRACTION;
        static const char *UNIFORM_NAME_BLUR_AXIS;
        static const char *UNIFORM_NAME_VIEWPROJECTION_MATRIX;
        static const char *UNIFORM_NAME_WINSIZE;
        static const char *UNIFORM_NAME_ISDIFFUSE;
        static const char *UNIFORM_NAME_ISGAMMA;

        virtual ~LightMap();

        LightMap() : Sprite(),
                     lightMapDrawingDisabled(false)
        {}

        static LightMap *create(LightHandler *lightHandler, int width, int height);

        virtual bool init(LightHandler *lightHandler, int width, int height);


        virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

        virtual void resizeRenderTexture(int width, int height);

        void gaussianBlur(Renderer *renderer, const Mat4 &transform, uint32_t flags);

        virtual void beginWithClearRenderTexture();

        virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;


        virtual void beginRenderTexture();
        virtual void endRenderTexture();

    };

}

#endif //PLATFORMER_LIGHTMAP_HPP
