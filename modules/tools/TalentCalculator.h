#pragma once
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>

class TalentCalculator : public QWidget {
    Q_OBJECT
public:
    TalentCalculator(Expansion exp, QWidget* parent = nullptr);
    
    void setCharacterClass(const QString& className);
    void setCharacterLevel(int level);
    void loadBuild(const QString& buildCode);
    QString getBuildCode() const;
    void resetTalents();
    
    struct Talent {
        int id;
        QString name;
        QString description;
        QString icon;
        int tier;
        int column;
        int maxRank;
        int currentRank;
        QList<int> dependencies;
        QList<int> exclusiveWith;
        QString spellId;
    };
    
    struct TalentTree {
        QString className;
        QString specialization;
        QList<Talent> talents;
        int pointsRequired[10]; // Puntos requeridos por tier
    };
    
signals:
    void talentPointsChanged(int spent, int available);
    void buildChanged(const QString& buildCode);
    
private slots:
    void onTalentClicked(int talentId);
    void onTalentRightClicked(int talentId);
    void onImportBuild();
    void onExportBuild();
    void onShareBuild();
    
private:
    void setupUI();
    void loadTalentData(Expansion exp, const QString& className);
    void updateTalentGraphics();
    void calculateAvailablePoints();
    bool validateTalentSelection(int talentId);
    void updateTalentConnections();
    
    QGraphicsScene* scene;
    QGraphicsView* view;
    TalentTree currentTree;
    QMap<int, QGraphicsItem*> talentItems;
    QMap<int, QGraphicsLineItem*> connectionLines;
    
    int availablePoints;
    int maxPoints;
    QString currentClass;
    Expansion expansion;
    
    // Widgets de control
    QLabel* pointsLabel;
    QPushButton* resetButton;
    QPushButton* importButton;
    QPushButton* exportButton;
    QComboBox* classSelector;
    QComboBox* specSelector;
};