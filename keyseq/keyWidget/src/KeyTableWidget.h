
#ifndef _KEYTABLE_WIDGET_H_
#define _KEYTABLE_WIDGET_H_

#include <QTableWidget>

class KeyTableWidget : public QTableWidget
{
    Q_OBJECT

    Q_PROPERTY(bool showResetBtns READ showResetBtns WRITE setShowResetBtns )

    bool m_bShowReset;

public:
    explicit KeyTableWidget(QWidget *parent = 0);

    void insertActRow(QAction* action);
    void insertTitleRow(QString title);

    void insertActRows(QList<QAction*> actions);
    void insertMenuRow(QMenu* menu);
    void appendWindow(class QMainWindow* window);

    void setDefault(QString action_name, QKeySequence default_sequence);

    bool showResetBtns() const;
    void setShowResetBtns(bool show);

};

#endif // _KEYTABLE_WIDGET_H_
