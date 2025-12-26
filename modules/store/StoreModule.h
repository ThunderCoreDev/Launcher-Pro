#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QWebEngineView>

class StoreModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.StoreModule")
    
public:
    StoreModule();
    ~StoreModule();
    
    QString getName() const override { return "StoreModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Tienda de donaciones y servicios"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { return {"DatabaseModule"}; }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Estructuras
    struct StoreItem {
        quint32 id;
        QString name;
        QString description;
        QString icon;
        QString category;
        QString type; // "item", "currency", "service", "package"
        QJsonObject data; // Datos específicos del tipo
        double price;
        QString currency; // "USD", "EUR", "points"
        int stock;
        bool limited;
        QDateTime availableFrom;
        QDateTime availableTo;
        bool featured;
        int sortOrder;
    };
    
    struct ShoppingCart {
        struct CartItem {
            StoreItem item;
            int quantity;
            double subtotal;
        };
        
        QList<CartItem> items;
        double total;
        QString currency;
        QString discountCode;
        double discountAmount;
        double finalTotal;
    };
    
    struct Order {
        quint64 id;
        QString orderNumber;
        quint32 accountId;
        QDateTime orderDate;
        double amount;
        QString currency;
        QString status; // "pending", "paid", "processing", "completed", "cancelled"
        QString paymentMethod;
        QJsonObject items;
        QString characterName;
        QString notes;
    };
    
    // Funciones principales
    void loadStoreItems();
    void addToCart(const StoreItem& item, int quantity = 1);
    void removeFromCart(quint32 itemId);
    void updateCartQuantity(quint32 itemId, int quantity);
    void applyDiscount(const QString& code);
    void checkout();
    void loadOrderHistory();
    
    ShoppingCart getCart() const { return m_cart; }
    QList<Order> getOrderHistory() const { return m_orders; }
    
signals:
    void cartUpdated(const ShoppingCart& cart);
    void orderPlaced(const Order& order);
    void paymentProcessed(const Order& order);
    void itemDelivered(quint32 accountId, const StoreItem& item, const QString& characterName);
    void discountApplied(const QString& code, double amount);
    
private slots:
    void onCategorySelected(QTreeWidgetItem* item, int column);
    void onItemSelected(QTableWidgetItem* item);
    void onAddToCartClicked();
    void onRemoveFromCartClicked();
    void onCheckoutClicked();
    void onApplyDiscountClicked();
    void onViewOrderDetails(quint64 orderId);
    void onWebViewLoaded(bool ok);
    void onPaymentCompleted(const QJsonObject& result);
    
private:
    void setupUI();
    void setupWebView();
    void loadCategories();
    void loadFeaturedItems();
    void updateCartDisplay();
    void processPayment();
    void deliverItems(const Order& order);
    void sendOrderConfirmation(const Order& order);
    
    // Widgets
    QTreeWidget* m_categoryTree;
    QTableWidget* m_itemsTable;
    QTableWidget* m_cartTable;
    QTableWidget* m_ordersTable;
    QWebEngineView* m_webView;
    QLabel* m_cartTotalLabel;
    QLineEdit* m_discountEdit;
    QPushButton* m_checkoutButton;
    
    // Datos
    QMap<QString, QList<StoreItem>> m_itemsByCategory;
    ShoppingCart m_cart;
    QList<Order> m_orders;
    QMap<QString, double> m_discounts;
    
    // Configuración de pagos
    QString m_paymentGateway;
    QString m_currency;
    double m_taxRate;
    
    Q_DISABLE_COPY(StoreModule)
};