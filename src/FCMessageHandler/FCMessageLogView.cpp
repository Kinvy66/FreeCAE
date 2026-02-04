/**
 * @file FCMessageLogView.cpp
 * @brief 日志窗口，用于显示全局的消息
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCMessageLogView.h"
#include "ui_FCMessageLogView.h"
#include <QDebug>
#include <QMenu>
#include <QItemSelectionModel>
#include <QClipboard>
#include <QApplication>
#include <QKeyEvent>
#include "FCMessageLogModel.h"

namespace FC 
{
FCMessageLogView::FCMessageLogView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FCMessageLogView)
{
    ui->setupUi(this);
    
    // 创建action
    _actionMessageLogShowInfo     = createAction("actionMessageLogShowInfo",
                                             ":/icon/icon/clear-message.svg",
                                             true,
                                             true);
    _actionMessageLogShowWarning  = createAction("actionMessageLogShowWarning",
                                                ":/icon/icon/messageTypeWarning.svg",
                                                true,
                                                true);
    _actionMessageLogShowCritical = createAction("actionMessageLogShowCritical",
                                                 ":/icon/icon/messageTypeError.svg",
                                                 true,
                                                 true);
    _actionMessageLogShowDebug = createAction("actionMessageLogShowCritical",
                                              ":/icon/icon/messageTypeDebug.svg",
                                              true,
                                              true);
    _actionMessageLogClear        = createAction("actionMessageLogClear", ":/icon/icon/clear-message.svg");
    _actionCopySelectMessage      = createAction("actionCopySelectMessage", ":/icon/icon/copy.svg");
    // 构建菜单
    
    //
    _model           = new FCMessageLogModel(this);
    _sortFilterModel = new FCMessageLogsSortFilterProxyModel(this);
    _sortFilterModel->setSourceModel(_model);
    ui->tableView->setModel(_sortFilterModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    
    QFontMetrics fm = ui->tableView->fontMetrics();
    // 高度为行高的1.2
    ui->tableView->verticalHeader()->setDefaultSectionSize(fm.lineSpacing() * 1.2);
    
    //
    ui->toolButtonInfo->setDefaultAction(_actionMessageLogShowInfo);
    ui->toolButtonWarning->setDefaultAction(_actionMessageLogShowWarning);
    ui->toolButtonCritial->setDefaultAction(_actionMessageLogShowCritical);
    ui->toolButtonDebug->setDefaultAction(_actionMessageLogShowDebug);
    ui->toolButtonClear->setDefaultAction(_actionMessageLogClear);
    //
    connect(_actionMessageLogShowInfo, &QAction::triggered, this, &FCMessageLogView::setEnableShowInfoMsg);
    connect(_actionMessageLogShowWarning, &QAction::triggered, this, &FCMessageLogView::setEnableShowWarningMsg);
    connect(_actionMessageLogShowCritical, &QAction::triggered, this, &FCMessageLogView::setEnableShowCriticalMsg);
    connect(_actionMessageLogShowDebug, &QAction::triggered, this, &FCMessageLogView::setEnableShowDebugMsg);    
    connect(_actionMessageLogClear, &QAction::triggered, this, &FCMessageLogView::clearAll);
    connect(_actionCopySelectMessage, &QAction::triggered, this, &FCMessageLogView::copySelectionMessageToClipBoard);
    connect(this, &FCMessageLogView::customContextMenuRequested, this, &FCMessageLogView::onCustomContextMenuRequested);
    ui->tableView->setWordWrap(true);
    connect(ui->tableView, &QTableView::clicked, this, &FCMessageLogView::onTableViewItemClicked);
    FC::FCMessageQueueProxy* messageQueue = &(_model->messageQueueProxy());
    connect(messageQueue, &FC::FCMessageQueueProxy::messageQueueAppended, this, &FCMessageLogView::onMessageAppended);
    
    
    retranslateUi();
}

FCMessageLogView::~FCMessageLogView()
{
    delete ui;
}

QAction* FCMessageLogView::createAction(const char* objname, const char* iconpath, bool checkable, bool checked)
{
    QAction* act = new QAction(this);
    act->setObjectName(QString::fromUtf8(objname));
    QIcon icon(iconpath);
    act->setIcon(icon);
    act->setCheckable(checkable);
    if (checkable) {
        act->setChecked(checked);
    }
    return act;
}

void FCMessageLogView::onCustomContextMenuRequested(const QPoint& pos)
{
    if (ui->tableView->underMouse()) {
        if (nullptr == _menu) {
            buildMenu();
        }
        _menu->exec(mapToGlobal(pos));
    }
}

void FCMessageLogView::onMessageAppended()
{
    if (isAutoScrollToButtom()) {
        ui->tableView->scrollToBottom();
    }
}

void FCMessageLogView::buildMenu()
{
    _menu = new QMenu(this);
    _menu->addAction(_actionCopySelectMessage);
    _menu->addSeparator();
    _menu->addAction(_actionMessageLogShowInfo);
    _menu->addAction(_actionMessageLogShowWarning);
    _menu->addAction(_actionMessageLogShowCritical);
    _menu->addAction(_actionMessageLogShowDebug);    
    _menu->addSeparator();
    _menu->addAction(_actionMessageLogClear);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

bool FCMessageLogView::isAutoScrollToButtom() const
{
    return mIsAutoScrollToButtom;
}

void FCMessageLogView::setAutoScrollToButtom(bool isAutoScrollToButtom)
{
    mIsAutoScrollToButtom = isAutoScrollToButtom;
}
/**
 * @brief 设置是否允许DebugMsg的显示
 * @param on
 */
void FCMessageLogView::setEnableShowDebugMsg(bool on)
{
    _sortFilterModel->setAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptDebugMsg, on);
    ui->tableView->update();
}
/**
 * @brief 检测是否允许DebugMsg的显示
 * @return
 */
bool FCMessageLogView::isEnableShowDebugMsg() const
{
    return _sortFilterModel->testAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptDebugMsg);
}
/**
 * @brief 设置是否允许WarningMsg的显示
 * @param on
 */
void FCMessageLogView::setEnableShowWarningMsg(bool on)
{
    _sortFilterModel->setAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptWarningMsg, on);
    ui->tableView->update();
}
/**
 * @brief 检测是否允许WarningMsg的显示
 * @return
 */
bool FCMessageLogView::isEnableShowWarningMsg() const
{
    return _sortFilterModel->testAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptWarningMsg);
}
/**
 * @brief 设置是否允许CriticalMsg的显示
 * @param on
 */
void FCMessageLogView::setEnableShowCriticalMsg(bool on)
{
    _sortFilterModel->setAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptCriticalMsg, on);
    ui->tableView->update();
}
/**
 * @brief 检测是否允许CriticalMsg的显示
 * @return
 */
bool FCMessageLogView::isEnableShowCriticalMsg() const
{
    return _sortFilterModel->testAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptCriticalMsg);
}
/**
 * @brief 设置是否允许FatalMsg的显示
 * @param on
 */
void FCMessageLogView::setEnableShowFatalMsg(bool on)
{
    _sortFilterModel->setAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptFatalMsg, on);
    ui->tableView->update();
}
/**
 * @brief 检测是否允许FatalMsg的显示
 * @return
 */
bool FCMessageLogView::isEnableShowFatalMsg() const
{
    return _sortFilterModel->testAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptFatalMsg);
}
/**
 * @brief 设置是否允许InfoMsg的显示
 * @param on
 */
void FCMessageLogView::setEnableShowInfoMsg(bool on)
{
    _sortFilterModel->setAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptInfoMsg, on);
    ui->tableView->update();
}

/**
 * @brief 点击后自动适应尺寸
 * @param index
 */
void FCMessageLogView::onTableViewItemClicked(const QModelIndex& index)
{
    QHeaderView* vh = ui->tableView->verticalHeader();
    if (vh && index.isValid()) {
        if (index.row() < vh->count()) {
            vh->setSectionResizeMode(index.row(), QHeaderView::ResizeToContents);
        }
    }
}

/**
 * @brief 清空所有消息
 */
void FCMessageLogView::clearAll()
{
    _model->clearAll();
}

/**
 * @brief 把选中的文本复制到剪切板
 */
void FCMessageLogView::copySelectionMessageToClipBoard()
{
    QItemSelectionModel* sm = ui->tableView->selectionModel();
    if (nullptr == sm) {
        return;
    }
    int cc = _sortFilterModel->columnCount();
    QSet< int > rowIndexs;
    QModelIndexList indexs = sm->selectedIndexes();
    QString text;
    for (const QModelIndex& i : qAsConst(indexs)) {
        if (!rowIndexs.contains(i.row())) {
            if (!rowIndexs.isEmpty()) {
                text += "\n";
            }
            rowIndexs.insert(i.row());
            QString line;
            for (int c = 0; c < cc; ++c) {
                line += _sortFilterModel->data(_sortFilterModel->index(i.row(), c)).toString();
                if (c != cc - 1) {
                    line += "\t";
                }
            }
            text += line;
        }
    }
    if (text.isEmpty()) {
        qDebug() << "copy nothing to clipboard";
        return;
    }
    QClipboard* appclip = QApplication::clipboard();
    if (appclip) {
        appclip->setText(text);
    }
    qDebug() << "copy to clipboard:" << text;
}

/**
 * @brief 选中所有
 */
void FCMessageLogView::selectAll()
{
    ui->tableView->selectAll();
}

/**
 * @brief 事件改变捕获
 * @param event
 */
void FCMessageLogView::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QWidget::changeEvent(event);
}

/**
 * @brief 设置文本
 */
void FCMessageLogView::retranslateUi()
{
    _actionMessageLogShowInfo->setText(tr("Info"));
    _actionMessageLogShowInfo->setToolTip(tr("Show Info Message"));
    _actionMessageLogShowWarning->setText(tr("Warning"));
    _actionMessageLogShowWarning->setToolTip(tr("Show Warning Message"));
    _actionMessageLogShowCritical->setText(tr("Critical"));
    _actionMessageLogShowCritical->setToolTip(tr("Show Critical Message"));
    _actionMessageLogShowDebug->setText(tr("Debug"));
    _actionMessageLogShowDebug->setToolTip(tr("Show Debug Message"));
    _actionMessageLogClear->setText(tr("Clear"));                  // cn:清空
    _actionMessageLogClear->setToolTip(tr("Clear All Messages"));  // cn:清空所有消息
    _actionCopySelectMessage->setText(tr("Copy"));                 // 复制
    _actionCopySelectMessage->setToolTip(tr("Copy Select Message"));
}

/**
 * @brief 处理快捷键
 * @param event
 */
void FCMessageLogView::keyPressEvent(QKeyEvent* event)
{
    if (event) {
        if (Qt::ControlModifier == event->modifiers()) {
            if (Qt::Key_C == event->key()) {
                // 复制
                copySelectionMessageToClipBoard();
                event->accept();
            } else if (Qt::Key_A == event->key()) {
                selectAll();
                event->accept();
            }
        }
    }
    QWidget::keyPressEvent(event);
}

/**
 * @brief 获取内部的action
 * @param ac
 * @return
 */
QAction* FCMessageLogView::getAction(FCMessageLogView::MessageLogActions ac) const
{
    switch (ac) {
    case ActionInfo:
        return _actionMessageLogShowInfo;
    case ActionWarning:
        return _actionMessageLogShowWarning;
    case ActionCritial:
        return _actionMessageLogShowCritical;
    case ActionDebug:
        return _actionMessageLogShowDebug;
    case ActionClear:
        return _actionMessageLogClear;
    case ActionCopy:
        return _actionCopySelectMessage;
    default:
        break;
    }
    return nullptr;
}
/**
 * @brief 检测是否允许InfoMsg的显示
 * @return
 */
bool FCMessageLogView::isEnableShowInfoMsg() const
{
    return _sortFilterModel->testAcceptMessageTypeFlag(FCMessageLogsSortFilterProxyModel::AcceptInfoMsg);
}

} // namespace FC

