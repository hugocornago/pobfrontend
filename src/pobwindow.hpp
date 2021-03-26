#include <QCache>
#include <QDir>
#include <QOpenGLWindow>
#include <QPainter>
#include <QStandardPaths>
#include <QTimer>
#include <memory>

#include "main.h"
#include "subscript.hpp"

class POBWindow : public QOpenGLWindow {
    Q_OBJECT
public:
    POBWindow() : stringCache(200) {
        QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        scriptPath = QDir::currentPath();
        scriptWorkDir = QDir::currentPath();
        basePath = QDir::currentPath();
        userPath = AppDataLocation;

        fontFudge = 0;

        connect(&repaintTimer, &QTimer::timeout, this, QOverload<>::of(&QOpenGLWindow::update));

        currentLayer = &layers[{0, 0}];
    }

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
    std::vector<std::unique_ptr<Cmd>>* currentLayer = nullptr;
    QList<std::shared_ptr<SubScript>> subScriptList;
    std::shared_ptr<QOpenGLTexture> white;
    QCache<QString, std::shared_ptr<QOpenGLTexture>> stringCache;
    QTimer repaintTimer;
};

extern POBWindow* pobwindow;
extern int dscount;
