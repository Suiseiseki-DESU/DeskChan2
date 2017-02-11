#pragma once

#include <QWidget>

class PluginClass;

class CharacterWidget final: public QWidget {
	Q_OBJECT
public:
	explicit CharacterWidget(PluginClass *plugin);
	~CharacterWidget() override;
	PluginClass *plugin() const { return m_plugin; }
	void displayBalloon(QWidget *widget);
	void displayMessage(const QString &text);

protected:
	void closeEvent(QCloseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	PluginClass *m_plugin;
	QPixmap m_characterPixmap;
	QPixmap *m_pixmap = nullptr;
	QWidget *m_balloonWidget = nullptr;
	QPoint m_clickPos;
	bool m_dragging = false;
	QString m_assetsDir;
	QString m_dataDir;

	void initialize();
	void initCallbacks();
	void updatePixmap();
	void moveToDefaultPosition();

};
