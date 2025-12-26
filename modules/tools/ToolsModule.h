#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QTabWidget>
#include <QProcess>
#include <QTreeWidget>

class ToolsModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.ToolsModule")
    
public:
    ToolsModule();
    ~ToolsModule();
    
    QString getName() const override { return "ToolsModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Herramientas y utilidades para WoW"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { return {"DatabaseModule"}; }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Sub-módulos
    class TalentCalculator;
    class GearPlanner;
    class MacroGenerator;
    class AddonManager;
    class SystemOptimizer;
    
    // Funciones principales
    void scanWoWDirectory();
    void repairGameFiles();
    void cleanCache();
    void optimizeSettings();
    void backupConfig();
    void restoreConfig();
    void updateAddons();
    void diagnoseProblems();
    
    struct WoWInstallation {
        QString path;
        QString version;
        QString expansion;
        quint64 size;
        QDateTime lastModified;
        bool isValid;
        QStringList detectedAddons;
    };
    
    WoWInstallation getCurrentInstallation() const { return m_installation; }
    
signals:
    void wowDirectoryScanned(const WoWInstallation& installation);
    void repairProgress(int percent, const QString& message);
    void repairComplete(bool success, const QString& message);
    void cacheCleaned(qint64 freedSpace);
    void optimizationComplete(const QJsonObject& results);
    void addonsUpdated(int updated, int total);
    void diagnosisComplete(const QJsonObject& report);
    
private slots:
    void onSelectDirectoryClicked();
    void onScanClicked();
    void onRepairClicked();
    void onCleanCacheClicked();
    void onOptimizeClicked();
    void onBackupClicked();
    void onRestoreClicked();
    void onUpdateAddonsClicked();
    void onDiagnoseClicked();
    
    void onRepairProcessOutput();
    void onRepairProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onAddonUpdateProgress(int current, int total);
    
private:
    void setupUI();
    void setupTalentCalculator();
    void setupGearPlanner();
    void setupMacroGenerator();
    void setupAddonManager();
    void setupSystemOptimizer();
    
    bool validateWoWDirectory(const QString& path);
    void scanAddons(const QString& path);
    void calculateDirectorySize(const QString& path);
    void runFileIntegrityCheck();
    void generateOptimizationProfile();
    void createRestorePoint();
    
    // Widgets principales
    QTabWidget* m_toolsTabs;
    
    // Talent Calculator
    QWidget* m_talentTab;
    // ... widgets específicos del talent calculator
    
    // Gear Planner
    QWidget* m_gearTab;
    // ... widgets específicos del gear planner
    
    // Macro Generator
    QWidget* m_macroTab;
    // ... widgets específicos del macro generator
    
    // Addon Manager
    QWidget* m_addonsTab;
    QTreeWidget* m_addonsTree;
    QPushButton* m_updateAddonsButton;
    QPushButton* m_enableAllButton;
    QPushButton* m_disableAllButton;
    
    // System Optimizer
    QWidget* m_optimizerTab;
    QTreeWidget* m_settingsTree;
    QPushButton* m_optimizeButton;
    QPushButton* m_backupButton;
    QPushButton* m_restoreButton;
    
    // Herramientas del sistema
    QWidget* m_systemTab;
    QPushButton* m_scanButton;
    QPushButton* m_repairButton;
    QPushButton* m_cleanButton;
    QPushButton* m_diagnoseButton;
    QLabel* m_statusLabel;
    QProgressBar* m_progressBar;
    
    // Datos
    WoWInstallation m_installation;
    QProcess* m_repairProcess;
    QMap<QString, QVariant> m_backupSettings;
    QList<QString> m_addonRepositories;
    
    // Temporizadores
    QTimer* m_progressTimer;
    
    Q_DISABLE_COPY(ToolsModule)
};