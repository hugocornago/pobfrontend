#include <QCache>
#include <QDir>
#include <QOpenGLWindow>
#include <QPainter>
#include <QStandardPaths>
#include <QTimer>
#include <memory>

#include "main.h"
#include "subscript.hpp"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
  //#include "luajit.h"
}


class POBWindow : public QOpenGLWindow {
    Q_OBJECT
public:
//    POBWindow(QWindow *parent = 0) : QOpenGLWindow(parent) {};
    POBWindow() : stringCache(200) {
//        QSurfaceFormat theformat(format());
//        format.setProfile(QSurfaceFormat::CompatibilityProfile);
/*        format.setDepthBufferSize(24);
        format.setStencilBufferSize(0);
        format.setGreenBufferSize(8);
        format.setRedBufferSize(8);
        format.setBlueBufferSize(8);*/
//        theformat.setAlphaBufferSize(8);
//        std::cout << theformat.hasAlpha() << std::endl;
//        setFormat(theformat);
        QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        scriptPath = QDir::currentPath();
        scriptWorkDir = QDir::currentPath();
        basePath = QDir::currentPath();
        userPath = AppDataLocation;

        fontFudge = 0;

        connect(&repaintTimer, &QTimer::timeout, this, QOverload<>::of(&QOpenGLWindow::update));
    }

//    POBWindow() : QOpenGLWindow() {
//    };
//    ~POBWindow() {};
//protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void subScriptFinished();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    template<typename ...FmtArgs>
    void LAssert(lua_State* L, int cond, const char* fmt, FmtArgs ...fmtArgs)
    {
      if (!cond) {
        LError(L, fmt, fmtArgs...);
      }
    }
    void LError(lua_State* L, const char* fmt, ...);
    int IsUserData(lua_State* L, int index, const char* metaName);

    void SetDrawLayer(int layer);
    void SetDrawLayer(int layer, int subLayer);
    void SetDrawSubLayer(int subLayer) {
        SetDrawLayer(curLayer, subLayer);
    }
    void AppendCmd(std::unique_ptr<Cmd> cmd);
    void DrawColor(const float col[4] = NULL);
    void DrawColor(uint32_t col);
    QString scriptPath;
    QString scriptWorkDir;
    QString basePath;
    QString userPath;
    int curLayer;
    int curSubLayer;
    int fontFudge;
    int width;
    int height;
    bool isDrawing;
    QString fontName;
    float drawColor[4];
    std::map<QPair<int, int>, std::vector<std::unique_ptr<Cmd>>> layers;
    QList<std::shared_ptr<SubScript>> subScriptList;
    std::shared_ptr<QOpenGLTexture> white;
    QCache<QString, std::shared_ptr<QOpenGLTexture>> stringCache;
    QTimer repaintTimer;
};
