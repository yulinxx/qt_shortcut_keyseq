#include "KeyTableWidget.h"
#include "KeyGrabWidget.h"

#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QHeaderView>
#include <QMainWindow>
#include <QDebug>

KeyTableWidget::KeyTableWidget(QWidget *parent) :
    QTableWidget(parent), m_bShowReset(false)
{
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionMode(NoSelection);

    insertColumn(0);
    insertColumn(1);
    insertColumn(2);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setEditTriggers(EditTrigger::NoEditTriggers);
}

void KeyTableWidget::insertActRow(QAction* action)
{
    int row = rowCount();
    insertRow(row);

    QTableWidgetItem* item = new QTableWidgetItem(action->icon(),action->iconText());
    item->setData(Qt::UserRole,action->objectName());
    setItem(row,0,item);

    KeyGrabWidget* pWidget = new KeyGrabWidget(action);
    setCellWidget(row, 1, pWidget);

    QToolButton *clear = new QToolButton();
    clear->setIcon(QIcon::fromTheme("edit-clear"));
    clear->setText(tr("Clear"));
    clear->setToolTip(tr("Clear shortcut for \"%1\"").arg(action->iconText()));
    connect(clear,SIGNAL(clicked()), pWidget, SLOT(slotClear()));
    setCellWidget(row,2,clear);
}

void KeyTableWidget::insertTitleRow(QString title)
{
    int row = rowCount();
    insertRow(row);
    QTableWidgetItem* pItem = new QTableWidgetItem(title);
    QFont f = pItem->font();
    f.setBold(true);
    pItem->setFont(f);

    //item->setFlags(item->flags()&(~Qt::ItemIsEditable));
    //item->setFlags(item->flags()&(~Qt::ItemIsSelectable));
    setItem(row, 0, pItem);
    setSpan(row, 0, 1, columnCount());
}

void KeyTableWidget::insertActRows(QList<QAction *> actions)
{
    foreach(QAction* act, actions)
        if ( !act->isSeparator() )
            insertActRow(act);
}

void KeyTableWidget::insertMenuRow(QMenu *menu)
{
    if ( !menu->actions().empty() )
    {
        insertTitleRow(menu->title().replace('&',""));
        insertActRows(menu->actions());
    }
}

void KeyTableWidget::appendWindow(QMainWindow *window)
{
    foreach(QMenu* menu, window->findChildren<QMenu*>())
        insertMenuRow(menu);

    resizeColumnsToContents();
}

void KeyTableWidget::setDefault(QString action_name, QKeySequence default_sequence)
{
    for ( int i = 0; i < rowCount(); i++ )
    {
        QTableWidgetItem* pItem = item(i,0);
        if ( pItem && pItem->data(Qt::UserRole).toString() == action_name )
        {
            KeyGrabWidget* pWidget = qobject_cast<KeyGrabWidget*>(cellWidget(i, 1));
            QWidget* pW = cellWidget(i, 3);
            if ( pWidget && pW )
            {
                pWidget->slotSetDefaultSequence(default_sequence);
            }
        }
    }
}

bool KeyTableWidget::showResetBtns() const
{
    return m_bShowReset;
}

void KeyTableWidget::setShowResetBtns(bool show)
{
    m_bShowReset = show;
    for ( int i = 0; i < rowCount(); i++ )
    {

        KeyGrabWidget* pWidget = qobject_cast<KeyGrabWidget*>(cellWidget(i,1));
        QWidget* pW = cellWidget(i,0);
        if ( pWidget && pW )
        {
            pW->setVisible(show);
            qDebug()<<i;
        }
    }
}
