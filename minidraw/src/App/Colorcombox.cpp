#include "Colorcombox.h"

#include <QAction>
#include <QGridLayout>
#include <QMenu>
#include <QPainter>
#include <QColorDialog>


const QColor colors[6][8] =
{
    {QColor(0, 0, 0, 255), QColor(170, 0, 0, 255), QColor(0, 85, 0, 255), QColor(170, 85, 0, 255),
    QColor(0, 170, 0, 255), QColor(170, 170, 0, 255), QColor(0, 255, 0, 255), QColor(170, 250, 0, 255)},

    {QColor(0, 0, 127, 255), QColor(170, 0, 127, 255), QColor(0, 85, 127, 255), QColor(170, 85, 127, 255),
    QColor(0, 170, 127, 255), QColor(170, 170, 127, 255), QColor(0, 255, 127, 255), QColor(170, 255, 127, 255)},

    {QColor(0, 0, 255, 255), QColor(170, 0, 255, 255), QColor(0, 85, 255, 255), QColor(170, 85, 255, 255),
    QColor(0, 170, 255, 255), QColor(170, 170, 255, 255), QColor(0, 255, 255, 255), QColor(170, 255, 255, 255)},

    {QColor(85, 0, 0, 255), QColor(255, 0, 0, 255), QColor(85, 85, 0, 255), QColor(255, 85, 0, 255),
    QColor(85, 170, 0, 255), QColor(255, 170, 0, 255), QColor(85, 255, 0, 255), QColor(255, 255, 0, 255)},

    {QColor(85, 0, 127, 255), QColor(255, 0, 127, 255), QColor(85, 85, 127, 255), QColor(255, 85, 127, 255),
    QColor(85, 170, 127, 255), QColor(255, 170, 127, 255), QColor(85, 255, 127, 255), QColor(255, 255, 127, 255)},

    {QColor(85, 0, 255, 255), QColor(255, 0, 255, 255), QColor(85, 85, 255, 255), QColor(255, 85, 255, 255),
    QColor(85, 170, 255, 255), QColor(255, 170, 255, 255), QColor(85, 255, 255, 255), QColor(255, 255, 255, 255)}
};



ColorCombox::ColorCombox(QWidget* parent) :
    QToolButton(parent)
{
    setPopupMode(QToolButton::InstantPopup);
    setMenu(createColorMenu(SLOT(OnColorChanged()), SLOT(OnShowColorBoard())));
    this->menu()->setStyleSheet("background-color:whitesmoke");
    setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(this, SIGNAL(sigColorChanged(QColor)), this, SLOT(changeColor(QColor)));
}


ColorCombox::~ColorCombox()
{

}

void ColorCombox::setButtonIcon(const QString& imageFile, QColor color)
{
    setIcon(createColorToolButtonIcon(imageFile, color));
}

QMenu* ColorCombox::createColorMenu(const char* slot, const char* slotColorBoard)
{
    // 选择其他颜色
    QToolButton* pBtnOtherColor = new QToolButton;
    pBtnOtherColor->setText(tr("other colors"));
    pBtnOtherColor->setFixedSize(QSize(142, 20));
    pBtnOtherColor->setAutoRaise(true);
    pBtnOtherColor->setToolTip(tr("other colors"));
    connect(pBtnOtherColor, SIGNAL(clicked()), this, slotColorBoard);

    // 基本色
    QGridLayout* pGridLayout = new QGridLayout;
    pGridLayout->setAlignment(Qt::AlignCenter);
    pGridLayout->setContentsMargins(0, 0, 0, 0);
    pGridLayout->setSpacing(2);

    for (int iRow = 0; iRow < 6; iRow++)
    {
        for (int iCol = 0; iCol < 8; iCol++)
        {
            QAction* action = new QAction(this);
            action->setData(colors[iRow][iCol]);
            action->setIcon(createColorIcon(colors[iRow][iCol]));
            connect(action, SIGNAL(triggered()), this, slot);

            QToolButton* pBtnColor = new QToolButton;
            pBtnColor->setFixedSize(QSize(16, 16));
            pBtnColor->setAutoRaise(true);
            pBtnColor->setDefaultAction(action);

            pGridLayout->addWidget(pBtnColor, iRow, iCol);
        }
    }

    QWidget* widget = new QWidget;
    widget->setLayout(pGridLayout);

    QVBoxLayout* pVLayout = new QVBoxLayout;
    pVLayout->addWidget(widget);
    pVLayout->addWidget(pBtnOtherColor);

    QMenu* colorMenu = new QMenu(this);
    colorMenu->setLayout(pVLayout);

    return colorMenu;
}

QIcon ColorCombox::createColorToolButtonIcon(const QString& imageFile, QColor color)
{
    QPixmap pixmap(16, 18);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPixmap image(imageFile);

    QRect target(0, 0, 16, 16);
    QRect source(0, 0, 16, 16);
    painter.fillRect(QRect(0, 13, 16, 4), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);

    //return QIcon("");
}

QIcon ColorCombox::createColorIcon(QColor color)
{
    QPixmap pixmap(16, 16);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 16, 16), color);

    return QIcon(pixmap);
}

void ColorCombox::OnColorChanged()
{
    QAction* pFillColorAction = new QAction(this);
    pFillColorAction = qobject_cast<QAction*>(sender());
    QColor color = qvariant_cast<QColor>(pFillColorAction->data());

    this->menu()->close();
    m_color = color;
    emit sigColorChanged(m_color);
}

void ColorCombox::OnShowColorBoard()
{
    this->menu()->close();
    this->setStyleSheet("background-color:whitesmoke");
    QColorDialog colordlg(this);
    QColor color = colordlg.getColor(m_color, this, tr("color dialogue"));
    if (!color.isValid())
    {
        emit sigColorChanged(m_color);
        return;
    }
    m_color = color;
    emit sigColorChanged(m_color);
}


void ColorCombox::changeColor(QColor color)
{
    QString strstyle = QString("background-color:rgb(%1,%2,%3);").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
    this->setStyleSheet(strstyle);
}

void ColorCombox::setColor(QRgb color)
{
    this->m_color = (QColor)color;
    emit sigColorChanged(m_color);
}

QRgb ColorCombox::getColor()
{
    QRgb mRgb = qRgb(m_color.red(), m_color.green(), m_color.blue());
    return mRgb;
}


