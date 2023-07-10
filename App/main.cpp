#include <QApplication>
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)  && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtAndroidExtras/QtAndroid>
#endif

#include "mainwindow.h"
#ifdef RABBITCOMMON
    #include "RabbitCommonDir.h"
    #include "RabbitCommonTools.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    a.setApplicationName("TransformCoordinate");
    a.setApplicationVersion(TransformCoordinate_VERSION);
    
    QTranslator translator;
    bool bRet = translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
                    + "/" + "TransformCoordinateApp_" + QLocale::system().name() + ".qm");
    if(bRet)
        qApp->installTranslator(&translator);
    qDebug() << "language:" << QLocale::system().name();
 
    a.setApplicationDisplayName(QObject::tr("Transform coordinate"));

    RabbitCommon::CTools::Instance()->Init();
#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/icon/App"));
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif
    MainWindow w;
#if defined(Q_OS_ANDROID)
    w.showMaximized();
#else    
    w.show();
#endif

    int nRet = a.exec();

    if(bRet)
        qApp->removeTranslator(&translator);

    return nRet;
}
