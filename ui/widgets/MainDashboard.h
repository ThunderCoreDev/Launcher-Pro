#ifndef MAINDASHBOARD_H
#define MAINDASHBOARD_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>

class MainDashboard : public QMainWindow {
    Q_OBJECT
    
public:
    MainDashboard(QWidget* parent = nullptr);
    
private:
    void setupUI();
    void setupMenuBar();
    void setupDockWidgets();
    
    QTabWidget* m_mainTabs;
    QDockWidget* m_statsDock;
    QDockWidget* m_friendsDock;
    QDockWidget* m_newsDock;
};

#endif // MAINDASHBOARD_H