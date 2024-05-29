#include <QApplication>
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)  && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtAndroidExtras/QtAndroid>
#endif

#include "mainwindow.h"
#ifdef RABBITCOMMON
    #include "RabbitCommonDir.h"
    #include "RabbitCommonTools.h"
    #include "FrmUpdater.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("TransformCoordinate");
    a.setApplicationVersion(TransformCoordinate_VERSION);

    RabbitCommon::CTools::Instance()->Init();
    QSharedPointer<QTranslator> translator =
        RabbitCommon::CTools::Instance()->InstallTranslator("TransformCoordinateApp"); 
    a.setApplicationDisplayName(QObject::tr("Transform coordinate"));

#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/icon/App"));
    if(a.arguments().length() > 1) {
        pUpdate->GenerateUpdateJson();
        pUpdate->GenerateUpdateXml();
        return 0;
    }
#endif
    MainWindow w;
#if defined(Q_OS_ANDROID)
    w.showMaximized();
#else    
    w.show();
#endif

    int nRet = a.exec();
    
    RabbitCommon::CTools::Instance()->RemoveTranslator(translator);
    RabbitCommon::CTools::Instance()->Clean();
    return nRet;
}
