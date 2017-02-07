#include <QBitmap>
#include <QDebug>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QLabel>
#include <QDir>
#include "Plugin.h"
#include "SettingsDialog.h"
#include "CharacterWidget.h"

CharacterWidget::CharacterWidget(PluginClass *plugin): QWidget(nullptr), m_plugin(plugin) {
	setWindowTitle("DeskChan");
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	m_plugin->sendMessage("core:query-plugin-dir", QVariant(), [this](const QVariant &data1) {
		m_assetsDir = data1.toMap()["path"].toString() + QDir::separator() + "assets";
		m_plugin->sendMessage("core:query-plugin-data-dir", QVariant(), [this](const QVariant &data2) {
			m_dataDir = data2.toMap()["path"].toString();
			initialize();
		});
	});
}

CharacterWidget::~CharacterWidget() {
	if (m_pixmap) delete m_pixmap;
	m_plugin->m_characterWidget = nullptr;
}

void CharacterWidget::closeEvent(QCloseEvent *event) {
	m_plugin->sendMessage("core:quit", QVariant());
	event->accept();
}

void CharacterWidget::paintEvent(QPaintEvent *event) {
	QPainter painter;
	painter.begin(this);
	painter.drawPixmap(0, 0, *m_pixmap);
	painter.end();
}

void CharacterWidget::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_clickPos = event->globalPos();
		m_dragging = true;
	} else if (event->button() == Qt::RightButton) {
		QMenu menu;
		QAction settingsAction("Settings...");
		connect(&settingsAction, &QAction::triggered, [this]() {
			m_plugin->m_settingsDialog->show();
		});
		QAction quitAction("Quit");
		connect(&quitAction, &QAction::triggered, [this]() {
			m_plugin->sendMessage("core:quit", QVariant());
		});
		menu.addAction(&settingsAction);
		menu.addSeparator();
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

void CharacterWidget::initialize() {
	QString pixmapFileName = m_assetsDir + QDir::separator() + "mashiro.png";
	m_characterPixmap.load(pixmapFileName);
	updatePixmap();
	show();
	initCallbacks();
}

void CharacterWidget::initCallbacks() {
	m_plugin->subscribe("gui:say", [this](const QString &sender, const QString &tag, const QVariant &data) {
		displayMessage(data.toMap()["text"].toString());
	});
	m_plugin->sendMessage("core:register-alternative", QMap<QString, QVariant>({
			{"srcTag", "dc:say"}, {"dstTag", "gui:say"}, {"priority", 100}
	}));
}

void CharacterWidget::updatePixmap() {
	if (m_pixmap) delete m_pixmap;
	float characterPixmapScale = 400.0f / m_characterPixmap.height();
	QSize pixmapSize((int)(m_characterPixmap.width() * characterPixmapScale),
					 (int)(m_characterPixmap.height() * characterPixmapScale));
	if (m_balloonWidget) {
		pixmapSize.setWidth(pixmapSize.width() + 400);
	}
	m_pixmap = new QPixmap(pixmapSize);
	m_pixmap->fill(Qt::transparent);
	QPainter painter;
	painter.begin(m_pixmap);
	QRect characterRect(0, 0, (int)(m_characterPixmap.width() * characterPixmapScale),
						(int)(m_characterPixmap.height() * characterPixmapScale));
	if (m_balloonWidget) {
		characterRect.moveLeft(400);
	}
	painter.drawPixmap(characterRect, m_characterPixmap);
	if (m_balloonWidget) {
		QPainterPath path;
		path.addRoundedRect(QRect(10, 10, 380, 200), 10, 10);
		painter.fillPath(path, Qt::white);
		painter.drawPath(path);
		m_balloonWidget->move(20, 20);
		m_balloonWidget->resize(360, 180);
	}
	painter.end();
	resize(m_pixmap->size());
	setMask(m_pixmap->mask());
	if (isVisible()) {
		repaint();
	}
}

void CharacterWidget::displayBalloon(QWidget *widget) {
	if (m_balloonWidget) delete m_balloonWidget;
	m_balloonWidget = widget;
	if (m_balloonWidget) {
		m_balloonWidget->setParent(this);
	}
	if (m_pixmap) updatePixmap();
	if (m_balloonWidget) {
		m_balloonWidget->setVisible(true);
	}
}

void CharacterWidget::displayMessage(const QString &text) {
	QLabel *label = nullptr;
	if (text.size() > 0) {
		label = new QLabel(text);
		label->setAlignment(Qt::AlignCenter);
	}
	displayBalloon(label);
}
