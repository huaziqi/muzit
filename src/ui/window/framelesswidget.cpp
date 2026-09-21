#include "framelesswidget.h"

namespace {
constexpr int kResizeBorder = 6;
constexpr int kWindowRadius = 4;
}

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(kResizeBorder, kResizeBorder,
                                   kResizeBorder, kResizeBorder);

    titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);

    contentLayout = new QHBoxLayout();
    mainLayout->addLayout(contentLayout, 1);
    contentLayout->setContentsMargins(0, 6, 0, 0);

    connect(titleBar, &TitleBar::buttonEvent,
            this, &FramelessWidget::titleBarEvent);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
}

FramelessWidget::~FramelessWidget() = default;

void FramelessWidget::titleBarEvent(const QString &signal)
{
    if (signal == "closeWindow") {
        close();
    } else if (signal == "miniWindow") {
        showMinimized();
    } else if (signal == "maxiWindow") {
        isMaximized() ? showNormal() : showMaximized();
    }
}

Qt::Edges FramelessWidget::resizeEdgesAt(const QPointF &position) const
{
    if (isMaximized())
        return {};

    Qt::Edges edges;
    if (position.x() <= kResizeBorder)
        edges |= Qt::LeftEdge;
    else if (position.x() >= width() - kResizeBorder)
        edges |= Qt::RightEdge;

    if (position.y() <= kResizeBorder)
        edges |= Qt::TopEdge;
    else if (position.y() >= height() - kResizeBorder)
        edges |= Qt::BottomEdge;

    return edges;
}

void FramelessWidget::updateResizeCursor(const QPointF &position)
{
    const Qt::Edges edges = resizeEdgesAt(position);

    if (edges == (Qt::LeftEdge | Qt::TopEdge)
        || edges == (Qt::RightEdge | Qt::BottomEdge)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (edges == (Qt::RightEdge | Qt::TopEdge)
               || edges == (Qt::LeftEdge | Qt::BottomEdge)) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (edges.testFlag(Qt::LeftEdge) || edges.testFlag(Qt::RightEdge)) {
        setCursor(Qt::SizeHorCursor);
    } else if (edges.testFlag(Qt::TopEdge) || edges.testFlag(Qt::BottomEdge)) {
        setCursor(Qt::SizeVerCursor);
    } else {
        unsetCursor();
    }
}

void FramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const Qt::Edges edges = resizeEdgesAt(event->position());
        if (edges != Qt::Edges{} && windowHandle()
            && windowHandle()->startSystemResize(edges)) {
            event->accept();
            return;
        }
    }

    QWidget::mousePressEvent(event);
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        updateResizeCursor(event->position());

    QWidget::mouseMoveEvent(event);
}

void FramelessWidget::leaveEvent(QEvent *event)
{
    unsetCursor();
    QWidget::leaveEvent(event);
}

void FramelessWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(245, 245, 239));
    painter.setPen(Qt::NoPen);
    const int radius = isMaximized() ? 0 : kWindowRadius;
    painter.drawRoundedRect(rect(), radius, radius);
    QWidget::paintEvent(event);
}

void FramelessWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        const int margin = isMaximized() ? 0 : kResizeBorder;
        mainLayout->setContentsMargins(margin, margin, margin, margin);
        unsetCursor();
        update();
    }

    QWidget::changeEvent(event);
}
