#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbusplayer.h"
#include <QFile>
#include <QModelIndex>
#include <QSettings>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    dbusplayer* player;
    void addFile(QString filename);
    QString URI;
    QFile currentList;
    QSettings* settings;
private slots:
    void on_actionAbout_Qt_triggered();
    void on_action_About_triggered();
    void on_actionP_reference_triggered();
    void AudioStop();
    void AudioPlay();
    void AudioEOS();
    void AudioFailed();
    void on_actionS_eek_triggered();
    void on_action_Reward_triggered();
    void on_action_Foward_triggered();
    void on_playlist_activated(QModelIndex index);
    void on_playlist_currentRowChanged(int currentRow);
    void on_actionMove_Bottom_triggered();
    void on_actionMove_Down_triggered();
    void on_action_Move_Top_triggered();
    void on_actionV_olume_Down_triggered();
    void on_action_Volume_Up_triggered();
    void on_actionMove_Up_triggered();
    void on_action_Last_File_triggered();
    void on_actionF_irst_File_triggered();
    void on_actionP_rivous_File_triggered();
    void on_action_Next_File_triggered();
    void on_actionP_ause_triggered();
    void on_action_Stop_triggered();
    void on_action_Play_triggered();
    void on_action_Quit_triggered();
    void on_actionRe_move_File_triggered();
    void on_actionA_dd_File_triggered();
    void on_actionSave_Playlist_As_triggered();
    void on_action_Save_Playlist_triggered();
    void on_actionOpen_P_laylist_triggered();
};

#endif // MAINWINDOW_H
