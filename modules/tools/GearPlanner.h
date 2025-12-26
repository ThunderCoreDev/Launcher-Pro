#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QWebEngineView>

class GearPlanner : public QWidget {
    Q_OBJECT
public:
    GearPlanner(Expansion exp, QWidget* parent = nullptr);
    
    void setCharacter(const QString& className, const QString& spec);
    void loadCharacterGear(quint64 characterGuid);
    void simulateDps(const QString& fightStyle = "Patchwerk");
    void compareGearSets(const QList<QString>& gearSets);
    void optimizeGear();
    
    struct Item {
        int entry;
        QString name;
        QString icon;
        int itemLevel;
        QString slot;
        QMap<QString, int> stats;
        QString source;
        float weight;
        bool isBestInSlot;
        QString setBonus;
    };
    
    struct GearSet {
        QString name;
        QMap<QString, Item> items;
        QMap<QString, float> totalStats;
        float simulatedDps;
        QString simulationCode;
    };
    
signals:
    void gearSetCalculated(const GearSet& gearSet);
    void simulationComplete(float dps, const QString& report);
    
private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onSimulateClicked();
    void onOptimizeClicked();
    void onCompareClicked();
    void onExportClicked();
    
private:
    void setupUI();
    void loadItemDatabase(Expansion exp);
    void loadBestInSlotLists();
    void calculateStatWeights();
    void runSimulation(const GearSet& gearSet);
    void updateCharacterModel();
    
    QWebEngineView* modelView;
    QTreeWidget* itemTree;
    QTableWidget* statsTable;
    QLabel* characterModel;
    
    QMap<int, Item> itemDatabase;
    QMap<QString, QList<Item>> bisLists;
    GearSet currentGearSet;
    QString currentClass;
    QString currentSpec;
    Expansion expansion;
    
    // Widgets de estadísticas
    QProgressBar* statBars[10];
    QLabel* statLabels[10];
    QLineEdit* weightEdits[10];
};