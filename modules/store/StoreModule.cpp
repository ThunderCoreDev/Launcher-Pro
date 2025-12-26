#include "StoreModule.h"
#include "core/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTreeWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

StoreModule::StoreModule(QObject* parent) 
    : QObject(parent) {
}

StoreModule::~StoreModule() {
    shutdown();
}

QString StoreModule::getName() const { 
    return "StoreModule"; 
}

QString StoreModule::getVersion() const { 
    return "1.0.0"; 
}

QString StoreModule::getDescription() const { 
    return "Tienda de donaciones y servicios"; 
}

QJsonObject StoreModule::getConfig() const {
    QJsonObject config;
    config["currency"] = "USD";
    config["tax_rate"] = 0.0;
    config["enable_cart"] = true;
    return config;
}

QWidget* StoreModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QHBoxLayout* mainLayout = new QHBoxLayout(widget);
    
    // Panel izquierdo: Categorías y productos
    QWidget* productsPanel = new QWidget();
    QVBoxLayout* productsLayout = new QVBoxLayout(productsPanel);
    
    // Categorías
    QLabel* categoriesLabel = new QLabel("🛒 Categorías");
    categoriesLabel->setStyleSheet("font-weight: bold;");
    
    m_categoriesTree = new QTreeWidget();
    m_categoriesTree->setHeaderLabel("Productos");
    m_categoriesTree->setStyleSheet(
        "QTreeWidget { border: 1px solid #2a3f6f; border-radius: 4px; }"
    );
    
    // Productos
    QLabel* productsLabel = new QLabel("📦 Productos");
    productsLabel->setStyleSheet("font-weight: bold;");
    
    m_productsTable = new QTableWidget();
    m_productsTable->setColumnCount(4);
    m_productsTable->setHorizontalHeaderLabels({
        "Nombre", "Precio", "Stock", "Acción"
    });
    m_productsTable->horizontalHeader()->setStretchLastSection(true);
    
    productsLayout->addWidget(categoriesLabel);
    productsLayout->addWidget(m_categoriesTree);
    productsLayout->addWidget(productsLabel);
    productsLayout->addWidget(m_productsTable);
    
    // Panel derecho: Carrito y detalles
    QWidget* cartPanel = new QWidget();
    QVBoxLayout* cartLayout = new QVBoxLayout(cartPanel);
    
    // Carrito
    QLabel* cartLabel = new QLabel("🛍️ Carrito de Compras");
    cartLabel->setStyleSheet("font-weight: bold;");
    
    m_cartTable = new QTableWidget();
    m_cartTable->setColumnCount(4);
    m_cartTable->setHorizontalHeaderLabels({
        "Producto", "Cantidad", "Precio", "Total"
    });
    
    // Total
    QHBoxLayout* totalLayout = new QHBoxLayout();
    QLabel* totalLabel = new QLabel("Total:");
    totalLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    
    m_totalLabel = new QLabel("$0.00");
    m_totalLabel->setStyleSheet("font-size: 18px; color: #00ff88; font-weight: bold;");
    
    totalLayout->addWidget(totalLabel);
    totalLayout->addWidget(m_totalLabel);
    totalLayout->addStretch();
    
    // Cupón
    QHBoxLayout* couponLayout = new QHBoxLayout();
    QLabel* couponLabel = new QLabel("Cupón:");
    m_couponEdit = new QLineEdit();
    m_couponEdit->setPlaceholderText("Código de descuento...");
    QPushButton* applyCouponButton = new QPushButton("Aplicar");
    
    couponLayout->addWidget(couponLabel);
    couponLayout->addWidget(m_couponEdit);
    couponLayout->addWidget(applyCouponButton);
    
    // Botones de acción
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* clearButton = new QPushButton("🗑️ Limpiar");
    QPushButton* checkoutButton = new QPushButton("💰 Pagar");
    checkoutButton->setStyleSheet(
        "QPushButton { background: #00cc66; color: white; font-weight: bold; }"
        "QPushButton:hover { background: #00ff88; }"
    );
    
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(checkoutButton);
    buttonLayout->addStretch();
    
    cartLayout->addWidget(cartLabel);
    cartLayout->addWidget(m_cartTable);
    cartLayout->addLayout(totalLayout);
    cartLayout->addLayout(couponLayout);
    cartLayout->addLayout(buttonLayout);
    
    // Añadir paneles al layout principal
    mainLayout->addWidget(productsPanel, 2);
    mainLayout->addWidget(cartPanel, 1);
    
    // Conectar señales
    connect(m_categoriesTree, &QTreeWidget::itemClicked, this, &StoreModule::onCategorySelected);
    connect(applyCouponButton, &QPushButton::clicked, this, &StoreModule::applyCoupon);
    connect(clearButton, &QPushButton::clicked, this, &StoreModule::clearCart);
    connect(checkoutButton, &QPushButton::clicked, this, &StoreModule::checkout);
    
    // Cargar datos iniciales
    loadCategories();
    loadProducts();
    
    return widget;
}

bool StoreModule::initialize() {
    LOG_INFO("StoreModule initialized");
    
    // Inicializar carrito
    m_cartTotal = 0.0;
    
    return true;
}

void StoreModule::shutdown() {
    LOG_INFO("StoreModule shutdown");
}

void StoreModule::update() {
    // Actualizar stock y precios
}

void StoreModule::loadCategories() {
    m_categoriesTree->clear();
    
    QTreeWidgetItem* mountsCategory = new QTreeWidgetItem(m_categoriesTree);
    mountsCategory->setText(0, "🐴 Mounts");
    mountsCategory->setData(0, Qt::UserRole, "mounts");
    
    QTreeWidgetItem* petsCategory = new QTreeWidgetItem(m_categoriesTree);
    petsCategory->setText(0, "🐾 Mascotas");
    petsCategory->setData(0, Qt::UserRole, "pets");
    
    QTreeWidgetItem* itemsCategory = new QTreeWidgetItem(m_categoriesTree);
    itemsCategory->setText(0, "⚔️ Items");
    itemsCategory->setData(0, Qt::UserRole, "items");
    
    QTreeWidgetItem* servicesCategory = new QTreeWidgetItem(m_categoriesTree);
    servicesCategory->setText(0, "🔧 Servicios");
    servicesCategory->setData(0, Qt::UserRole, "services");
    
    QTreeWidgetItem* currencyCategory = new QTreeWidgetItem(m_categoriesTree);
    currencyCategory->setText(0, "💰 Moneda");
    currencyCategory->setData(0, Qt::UserRole, "currency");
}

void StoreModule::loadProducts() {
    // Limpiar tabla
    m_productsTable->setRowCount(0);
    
    // Añadir productos de ejemplo
    addProductToTable("Ashes of Al'ar", 25.00, 10, "mounts");
    addProductToTable("Invincible", 30.00, 5, "mounts");
    addProductToTable("Mimirón's Head", 20.00, 15, "mounts");
    addProductToTable("Banco de 10K de Oro", 10.00, -1, "currency");
    addProductToTable("Banco de 50K de Oro", 40.00, -1, "currency");
    addProductToTable("Cambio de Nombre", 15.00, -1, "services");
    addProductToTable("Cambio de Facción", 30.00, -1, "services");
    addProductToTable("Cambio de Raza", 25.00, -1, "services");
}

void StoreModule::addProductToTable(const QString& name, double price, int stock, const QString& category) {
    int row = m_productsTable->rowCount();
    m_productsTable->insertRow(row);
    
    // Nombre
    QTableWidgetItem* nameItem = new QTableWidgetItem(name);
    m_productsTable->setItem(row, 0, nameItem);
    
    // Precio
    QTableWidgetItem* priceItem = new QTableWidgetItem(QString("$%1").arg(price, 0, 'f', 2));
    priceItem->setTextAlignment(Qt::AlignRight);
    m_productsTable->setItem(row, 1, priceItem);
    
    // Stock
    QString stockText = stock == -1 ? "∞" : QString::number(stock);
    QTableWidgetItem* stockItem = new QTableWidgetItem(stockText);
    stockItem->setTextAlignment(Qt::AlignCenter);
    m_productsTable->setItem(row, 2, stockItem);
    
    // Botón de añadir al carrito
    QWidget* actionWidget = new QWidget();
    QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(2, 2, 2, 2);
    
    QSpinBox* quantitySpin = new QSpinBox();
    quantitySpin->setMinimum(1);
    quantitySpin->setMaximum(stock == -1 ? 100 : stock);
    quantitySpin->setValue(1);
    
    QPushButton* addButton = new QPushButton("➕");
    addButton->setFixedSize(30, 25);
    
    actionLayout->addWidget(quantitySpin);
    actionLayout->addWidget(addButton);
    actionLayout->addStretch();
    
    m_productsTable->setCellWidget(row, 3, actionWidget);
    
    // Conectar botón
    connect(addButton, &QPushButton::clicked, this, [this, name, price, quantitySpin]() {
        addToCart(name, price, quantitySpin->value());
    });
}

void StoreModule::onCategorySelected(QTreeWidgetItem* item, int column) {
    if (!item) return;
    
    QString category = item->data(0, Qt::UserRole).toString();
    LOG_INFO("Category selected: " + category);
    
    // Filtrar productos por categoría
    // (En implementación real, cargarías de la BD)
}

void StoreModule::addToCart(const QString& productName, double price, int quantity) {
    LOG_INFO(QString("Adding to cart: %1 x%2 ($%3)").arg(productName).arg(quantity).arg(price));
    
    double itemTotal = price * quantity;
    m_cartTotal += itemTotal;
    
    // Añadir a la tabla del carrito
    int row = m_cartTable->rowCount();
    m_cartTable->insertRow(row);
    
    m_cartTable->setItem(row, 0, new QTableWidgetItem(productName));
    m_cartTable->setItem(row, 1, new QTableWidgetItem(QString::number(quantity)));
    m_cartTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(price, 0, 'f', 2)));
    m_cartTable->setItem(row, 3, new QTableWidgetItem(QString("$%1").arg(itemTotal, 0, 'f', 2)));
    
    // Actualizar total
    m_totalLabel->setText(QString("$%1").arg(m_cartTotal, 0, 'f', 2));
}

void StoreModule::applyCoupon() {
    QString couponCode = m_couponEdit->text().trimmed();
    if (!couponCode.isEmpty()) {
        LOG_INFO("Applying coupon: " + couponCode);
        // Implementar lógica de cupón
    }
}

void StoreModule::clearCart() {
    m_cartTable->setRowCount(0);
    m_cartTotal = 0.0;
    m_totalLabel->setText("$0.00");
    LOG_INFO("Cart cleared");
}

void StoreModule::checkout() {
    if (m_cartTable->rowCount() == 0) {
        LOG_WARNING("Cannot checkout: cart is empty");
        return;
    }
    
    LOG_INFO("Processing checkout...");
    // Implementar proceso de pago
}