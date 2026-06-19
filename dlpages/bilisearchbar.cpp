#include "bilisearchbar.h"

BiliSearchBar::BiliSearchBar(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 6, 8, 6);
    mainLayout->setSpacing(6);

    // 搜索类型切换按钮组
    typeGroup   = new QButtonGroup(this);
    keywordBtn  = new QPushButton("关键词");
    bvidBtn     = new QPushButton("BV号");
    for (auto *btn : {keywordBtn, bvidBtn}) {
        btn->setCheckable(true);
        btn->setFixedHeight(28);
        btn->setMinimumWidth(56);
    }
    keywordBtn->setChecked(true);
    typeGroup->addButton(keywordBtn, static_cast<int>(BiliSearchType::Keyword));
    typeGroup->addButton(bvidBtn,    static_cast<int>(BiliSearchType::BvId));
    typeGroup->setExclusive(true);

    // 搜索框
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("搜索视频标题 / 输入 BVxxxxxxx");
    searchInput->setFixedHeight(28);
    connect(searchInput, &QLineEdit::returnPressed, this, &BiliSearchBar::onSearchClicked);

    // 搜索按钮
    searchBtn = new QPushButton("搜索");
    searchBtn->setFixedSize(60, 28);
    connect(searchBtn, &QPushButton::clicked, this, &BiliSearchBar::onSearchClicked);

    // 每页条数
    QLabel *pageSizeLabel = new QLabel("每页");
    pageSizeLabel->setFixedHeight(28);
    pageSizeBox = new QComboBox();
    pageSizeBox->addItems({"10", "20", "30"});
    pageSizeBox->setFixedSize(50, 28);
    QLabel *tiao = new QLabel("条");
    tiao->setFixedHeight(28);

    mainLayout->addWidget(keywordBtn);
    mainLayout->addWidget(bvidBtn);
    mainLayout->addWidget(searchInput, 1);
    mainLayout->addWidget(searchBtn);
    mainLayout->addWidget(pageSizeLabel);
    mainLayout->addWidget(pageSizeBox);
    mainLayout->addWidget(tiao);
}

void BiliSearchBar::onSearchClicked()
{
    QString keyword = searchInput->text().trimmed();
    if (keyword.isEmpty()) return;
    auto type = static_cast<BiliSearchType>(typeGroup->checkedId());
    int pageSize = pageSizeBox->currentText().toInt();
    emit searchRequested(keyword, type, pageSize);
}
