#include <QDebug>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QDir>
#include "Plugin.h"
#include "CharacterWidget.h"

CharacterWidget::CharacterWidget(PluginClass *plugin): QWidget(nullptr), m_plugin(plugin) {
	setWindowTitle("DeskChan");
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	m_plugin->sendMessage("core:query-plugin-dir", QVariant(), [this](const QVariant &data1) {
		m_assetsDir = data1.toMap()["path"].toString() + QDir::separator() + "assets";
		m_plugin->sendMessage("core:query-plugin-data-dir", QVariant(), [this](const QVariant &data2) {
			m_dataDir = data2.toMap()["path"].toString();
			QString pixmapFileName = m_assetsDir + QDir::separator() + "mashiro.png";
			m_pixmap.load(pixmapFileName);
			float ratio = (float)m_pixmap.width() / m_pixmap.height();
			resize((int)(700 * ratio), (int)(700 * (1 - ratio)));
			show();
			initCallbacks();
			m_plugin->sendMessage("core:register-alternative", QMap<QString, QVariant>({
					{"srcTag", "dc:say"}, {"dstTag", "gui:say"}, {"priority", 100}
			}));
		});
	});
}

CharacterWidget::~CharacterWidget() {
	m_plugin->m_characterWidget = nullptr;
}

void CharacterWidget::closeEvent(QCloseEvent *event) {
	m_plugin->sendMessage("core:quit", QVariant());
	event->accept();
}

void CharacterWidget::paintEvent(QPaintEvent *event) {
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	float pixmapScale = (float)height() / m_pixmap.height();
	QRect pixmapRect = {0, 0, (int)(m_pixmap.width() * pixmapScale), (int)(m_pixmap.height() * pixmapScale)};
	pixmapRect.moveCenter(QPoint(width() / 2, height() / 2));
	painter.drawPixmap(pixmapRect, m_pixmap);
	painter.end();
}

void CharacterWidget::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_clickPos = event->globalPos();
		m_dragging = true;
	} else if (event->button() == Qt::RightButton) {
		QMenu menu;
		QAction quitAction("Quit");
		connect(&quitAction, &QAction::triggered, [this]() {
			m_plugin->sendMessage("core:quit", QVariant());
		});
		menu.addAction(&quitAction);
		menu.exec(event->globalPos());
	}
}

void CharacterWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_dragging = false;
	}
}

void CharacterWidget::mouseMoveEvent(QMouseEvent *event) {
	if (m_dragging) {
		QPoint delta = event->globalPos() - m_clickPos;
		move(pos() + delta);
		m_clickPos = event->globalPos();
	}
}

void CharacterWidget::initCallbacks() {
	m_plugin->subscribe("gui:say", [this](const QString &sender, const QString &tag, const QVariant &data) {
		//
	});
}
