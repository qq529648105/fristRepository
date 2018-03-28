#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QDialog>

namespace Ui {
class CameraManager;
}

class CameraManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraManager(QString id,QWidget *parent = 0);
    ~CameraManager();
    
private slots:
    void on_ButtonAdd_clicked();
    void on_ButtonDel_clicked();
    void on_ButtonUpdate_clicked();
    void updateValue(int,int);

private:
    Ui::CameraManager *ui;
    QString solutionId;
    void initCameraTable();
    void addCameraRow(const QString& cameraId);
signals:
    void signalAddCamera(QString &cameraId);
    void signalDelCamera(QString &cameraId);
    void signalUpdateCamera(QString &cameraId);
};

#endif // CAMERAMANAGER_H
