#pragma once
#include <QObject>
#include <QMap>
#include <QJsonObject>
#include <QSettings>

struct ThemeConfig {
    QString name;
    QString displayName;
    QString author;
    QString version;
    
    // Colores principales
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString surfaceColor;
    QString textColor;
    QString textSecondaryColor;
    
    // Propiedades visuales
    int borderRadius;
    int animationSpeed;
    bool enableShadows;
    bool enableBlur;
    bool enableAnimations;
    
    // Recursos
    QString backgroundImage;
    QString backgroundAnimation;
    QString buttonStyle;
    QString fontFamily;
    
    // Sonidos
    QString clickSound;
    QString notificationSound;
    QString loginSound;
    
    // Efectos especiales
    bool enableParticles;
    QString particleEffect;
    int particleDensity;
    
    // Modo
    bool isDarkMode;
    
    QJsonObject toJson() const;
    static ThemeConfig fromJson(const QJsonObject& json);
};

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager& instance();
    
    void loadThemes();
    void setCurrentTheme(const QString& themeName);
    ThemeConfig getCurrentTheme() const;
    QList<ThemeConfig> getAvailableThemes() const;
    
    void applyTheme(QWidget* widget);
    QString getStyleSheet() const;
    
    // Personalización dinámica
    void setCustomColor(const QString& colorName, const QColor& color);
    void setFontSize(int size);
    void setAnimationSpeed(int speed);
    void setTransparency(int alpha);
    
    // Efectos especiales
    void enableBlurEffect(QWidget* widget, bool enable);
    void enableShadowEffect(QWidget* widget, bool enable);
    void startParticleEffect(QWidget* parent);
    
signals:
    void themeChanged(const ThemeConfig& newTheme);
    void customizationUpdated();
    
private:
    ThemeManager();
    void loadBuiltinThemes();
    void loadCustomThemes();
    void saveTheme(const ThemeConfig& theme);
    
    QMap<QString, ThemeConfig> themes;
    QString currentThemeName;
    QSettings settings;
};