#include "wiimoterawstream.h"
#include "ui_wiimoterawstream.h"

#include <QComboBox>
#include <QLabel>
#include <QProgressBar>

#include "../config.h"

WiimoteRawStream::WiimoteRawStream(WiimotedevDeviceEvents *iface, InfraredCameraView *graphics, QWidget *parent)
		: QMainWindow(parent)
		, ui(new Ui::WiimoteRawStream)
		, m_interface(iface)
		, m_id(1)
		, m_mainWindow(graphics) {
	ui->setupUi(this);
	QLabel *label;
	QObject::connect(iface, SIGNAL(dbusClassicControllerPlugged(uint)), this, SLOT(dbusClassicPlugged(uint)));
	QObject::connect(iface, SIGNAL(dbusClassicControllerUnplugged(uint)), this, SLOT(dbusClassicUnplugged(uint)));
	QObject::connect(iface, SIGNAL(dbusClassicControllerRStick(uint, const stickdata &)), this, SLOT(dbusClassicControllerRStick(uint, const stickdata &)));
	QObject::connect(iface, SIGNAL(dbusClassicControllerLStick(uint, const stickdata &)), this, SLOT(dbusClassicControllerLStick(uint, const stickdata &)));
	QObject::connect(iface, SIGNAL(dbusNunchukAcc(uint, const accdata &)), this, SLOT(dbusNunchukAcc(uint, const accdata &)));
	QObject::connect(iface, SIGNAL(dbusNunchukPlugged(uint)), this, SLOT(dbusNunchukPlugged(uint)));
	QObject::connect(iface, SIGNAL(dbusNunchukUnplugged(uint)), this, SLOT(dbusNunchukUnplugged(uint)));
	QObject::connect(iface, SIGNAL(dbusNunchukStick(uint, const stickdata &)), this, SLOT(dbusNunchukStick(uint, const stickdata &)));
	QObject::connect(iface, SIGNAL(dbusVirtualCursorPosition(uint, double, double, double, double)), this, SLOT(dbusVirtualCursorPosition(uint, double, double, double, double)));
	QObject::connect(iface, SIGNAL(dbusVirtualCursorFound(uint)), this, SLOT(dbusVirtualCursorFound(uint)));
	QObject::connect(iface, SIGNAL(dbusVirtualCursorLost(uint)), this, SLOT(dbusVirtualCursorLost(uint)));
	QObject::connect(iface, SIGNAL(dbusWiimoteConnected(uint)), this, SLOT(dbusWiimoteConnected(uint)));
	QObject::connect(iface, SIGNAL(dbusWiimoteDisconnected(uint)), this, SLOT(dbusWiimoteDisconnected(uint)));
	QObject::connect(iface, SIGNAL(dbusWiimoteAcc(uint, const accdata &)), this, SLOT(dbusWiimoteAcc(uint, const accdata &)));
	QObject::connect(iface, SIGNAL(dbusWiimoteBatteryLife(uint, uint8)), this, SLOT(dbusWiimoteBatteryLife(uint, uint8)));
	QObject::connect(iface, SIGNAL(dbusWiimoteGeneralButtons(uint, uint64)), this, SLOT(dbusWiimoteGeneralButtons(uint, uint64)));
	QObject::connect(iface, SIGNAL(dbusWiimoteInfrared(uint, const QList<irpoint> &)), this, SLOT(dbusWiimoteInfrared(uint, const QList<struct irpoint> &)));
	QObject::connect(iface, SIGNAL(dbusWiimoteLedStatusChanged(uint, uint8)), this, SLOT(dbusWiimoteLedStatusChanged(uint, uint8)));
	QObject::connect(iface, SIGNAL(dbusWiimoteRumbleStatusChanged(uint, uint8)), this, SLOT(dbusWiimoteRumbleStatusChanged(uint, uint8)));
	QFont font("Monospace", 8);
	font.setStyleHint(QFont::TypeWriter);
	ui->treeWidget->setFont(font);
	ui->topTool->addWidget(label = new QLabel());
	label->setPixmap(QPixmap(":/icon32.png"));
	label->setMargin(1);
	QWidget *widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_wiimoteComboBox = new QComboBox());
	layout->addWidget(m_wiimoteBatteryProgressBar = new QProgressBar());
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->setSpacing(0);
	widget->setLayout(layout);
	widget->setMaximumWidth(180);
	ui->topTool->addWidget(widget);
	m_wiimoteBatteryProgressBar->setFormat("Battery: %p%");
	ui->topTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	m_classicConnected = ui->topTool->addAction(QIcon(":/controller_active.png"), "Classic[*]");
	m_nunchukConnected = ui->topTool->addAction(QIcon(":/nunchuk_active.png"), "Nunchuk[*]");
	ui->topTool->addSeparator();
	m_wiimoteRumble = ui->topTool->addAction(QIcon(":/rumble_off.png"), "Rumble[ ]");
	ui->topTool->addSeparator();
	m_cursorVisible = ui->topTool->addAction(QIcon(":/cursor_disabled.png"), "Cursor[ ]");
	m_wiimotePoints[0] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Point[1]");
	m_wiimotePoints[1] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Point[2]");
	m_wiimotePoints[2] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Point[3]");
	m_wiimotePoints[3] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Point[4]");
	m_wiimotePoints[0]->setData(false);
	m_wiimotePoints[1]->setData(false);
	m_wiimotePoints[2]->setData(false);
	m_wiimotePoints[3]->setData(false);
	ui->topTool->addSeparator();
	m_wiimoteLeds[0] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Led[1]");
	m_wiimoteLeds[1] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Led[2]");
	m_wiimoteLeds[2] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Led[3]");
	m_wiimoteLeds[3] = ui->topTool->addAction(QIcon(":/disabled24.png"), "Led[4]");
	m_wiimoteLeds[0]->setData(false);
	m_wiimoteLeds[1]->setData(false);
	m_wiimoteLeds[2]->setData(false);
	m_wiimoteLeds[3]->setData(false);
	opts << new QTreeWidgetItem();
	opts.last()->setText(0, "Accelerometer");
	opts.last()->setText(1, "                  ");
	opts.last()->addChild(m_accelerometerItems[0][0] = new QTreeWidgetItem(QStringList("X-Axis")));
	opts.last()->addChild(m_accelerometerItems[0][1] = new QTreeWidgetItem(QStringList("Y-Axis")));
	opts.last()->addChild(m_accelerometerItems[0][2] = new QTreeWidgetItem(QStringList("Z-Axis")));
	opts.last()->addChild(m_accelerometerItems[0][3] = new QTreeWidgetItem(QStringList("Pitch")));
	opts.last()->addChild(m_accelerometerItems[0][4] = new QTreeWidgetItem(QStringList("Roll")));
	opts << new QTreeWidgetItem();
	opts.last()->setText(0, "Buttons");
	QTreeWidgetItem *wiimoteDPad = new QTreeWidgetItem(QStringList("Digital Pad"));
	QTreeWidgetItem *wiimoteShift = new QTreeWidgetItem(QStringList("Shift"));
	QTreeWidgetItem *wiimoteTilt = new QTreeWidgetItem(QStringList("Tilt"));
	opts.last()->addChild(m_wiimoteButtonItems[0] = new QTreeWidgetItem(QStringList("1")));
	opts.last()->addChild(m_wiimoteButtonItems[1] = new QTreeWidgetItem(QStringList("2")));
	opts.last()->addChild(m_wiimoteButtonItems[2] = new QTreeWidgetItem(QStringList("A")));
	opts.last()->addChild(m_wiimoteButtonItems[3] = new QTreeWidgetItem(QStringList("B")));
	opts.last()->addChild(m_wiimoteButtonItems[4] = new QTreeWidgetItem(QStringList("Minus")));
	opts.last()->addChild(m_wiimoteButtonItems[5] = new QTreeWidgetItem(QStringList("Plus")));
	opts.last()->addChild(m_wiimoteButtonItems[6] = new QTreeWidgetItem(QStringList("Home")));
	wiimoteDPad->addChild(m_wiimoteButtonItems[7] = new QTreeWidgetItem(QStringList("Right")));
	wiimoteDPad->addChild(m_wiimoteButtonItems[8] = new QTreeWidgetItem(QStringList("Left")));
	wiimoteDPad->addChild(m_wiimoteButtonItems[9] = new QTreeWidgetItem(QStringList("Down")));
	wiimoteDPad->addChild(m_wiimoteButtonItems[10] = new QTreeWidgetItem(QStringList("Up")));
	wiimoteShift->addChild(m_wiimoteButtonItems[11] = new QTreeWidgetItem(QStringList("Backward")));
	wiimoteShift->addChild(m_wiimoteButtonItems[12] = new QTreeWidgetItem(QStringList("Forward")));
	wiimoteShift->addChild(m_wiimoteButtonItems[13] = new QTreeWidgetItem(QStringList("Right")));
	wiimoteShift->addChild(m_wiimoteButtonItems[14] = new QTreeWidgetItem(QStringList("Left")));
	wiimoteShift->addChild(m_wiimoteButtonItems[15] = new QTreeWidgetItem(QStringList("Down")));
	wiimoteShift->addChild(m_wiimoteButtonItems[16] = new QTreeWidgetItem(QStringList("Up")));
	wiimoteTilt->addChild(m_wiimoteButtonItems[17] = new QTreeWidgetItem(QStringList("Front")));
	wiimoteTilt->addChild(m_wiimoteButtonItems[18] = new QTreeWidgetItem(QStringList("Back")));
	wiimoteTilt->addChild(m_wiimoteButtonItems[19] = new QTreeWidgetItem(QStringList("Right")));
	wiimoteTilt->addChild(m_wiimoteButtonItems[20] = new QTreeWidgetItem(QStringList("Left")));
	opts.last()->addChild(wiimoteDPad);
	opts.last()->addChild(wiimoteShift);
	opts.last()->addChild(wiimoteTilt);
	opts << new QTreeWidgetItem();
	opts.last()->setText(0, "Infrared");
	opts.last()->addChild(m_infraredItems[0] = new QTreeWidgetItem(QStringList("Point [1]")));
	opts.last()->addChild(m_infraredItems[1] = new QTreeWidgetItem(QStringList("Point [2]")));
	opts.last()->addChild(m_infraredItems[2] = new QTreeWidgetItem(QStringList("Point [3]")));
	opts.last()->addChild(m_infraredItems[3] = new QTreeWidgetItem(QStringList("Point [4]")));
	QTreeWidgetItem *wiimoteVC = new QTreeWidgetItem();
	wiimoteVC->setText(0, "Virtual Cursor");
	wiimoteVC->addChild(m_infraredItems[4] = new QTreeWidgetItem(QStringList("Visible")));
	wiimoteVC->addChild(m_infraredItems[5] = new QTreeWidgetItem(QStringList("Point")));
	wiimoteVC->addChild(m_infraredItems[6] = new QTreeWidgetItem(QStringList("Angle")));
	wiimoteVC->addChild(m_infraredItems[7] = new QTreeWidgetItem(QStringList("Distance")));
	opts.last()->addChild(wiimoteVC);
	opts << new QTreeWidgetItem();
	opts.last()->setText(0, "Extensions");
	QTreeWidgetItem *nunchuk = new QTreeWidgetItem();
	nunchuk->setText(0, "Nunchuk");
	opts.last()->addChild(nunchuk);
	QTreeWidgetItem *nunchukAcc = new QTreeWidgetItem();
	nunchukAcc->setText(0, "Accelerometer");
	nunchuk->addChild(nunchukAcc);
	nunchukAcc->addChild(m_accelerometerItems[1][0] = new QTreeWidgetItem(QStringList("X-Axis")));
	nunchukAcc->addChild(m_accelerometerItems[1][1] = new QTreeWidgetItem(QStringList("Y-Axis")));
	nunchukAcc->addChild(m_accelerometerItems[1][2] = new QTreeWidgetItem(QStringList("Z-Axis")));
	nunchukAcc->addChild(m_accelerometerItems[1][3] = new QTreeWidgetItem(QStringList("Pitch")));
	nunchukAcc->addChild(m_accelerometerItems[1][4] = new QTreeWidgetItem(QStringList("Roll")));
	QTreeWidgetItem *nunchukButtons = new QTreeWidgetItem();
	nunchukButtons->setText(0, "Buttons");
	QTreeWidgetItem *nunchukStick = new QTreeWidgetItem(QStringList("Stick"));
	QTreeWidgetItem *nunchukShift = new QTreeWidgetItem(QStringList("Shift"));
	QTreeWidgetItem *nunchukTilt = new QTreeWidgetItem(QStringList("Tilt"));
	nunchukButtons->addChild(m_wiimoteButtonItems[21] = new QTreeWidgetItem(QStringList("C")));
	nunchukButtons->addChild(m_wiimoteButtonItems[22] = new QTreeWidgetItem(QStringList("Z")));
	nunchukStick->addChild(m_wiimoteButtonItems[23] = new QTreeWidgetItem(QStringList("Right")));
	nunchukStick->addChild(m_wiimoteButtonItems[24] = new QTreeWidgetItem(QStringList("Left")));
	nunchukStick->addChild(m_wiimoteButtonItems[25] = new QTreeWidgetItem(QStringList("Down")));
	nunchukStick->addChild(m_wiimoteButtonItems[26] = new QTreeWidgetItem(QStringList("Up")));
	nunchukShift->addChild(m_wiimoteButtonItems[27] = new QTreeWidgetItem(QStringList("Backward")));
	nunchukShift->addChild(m_wiimoteButtonItems[28] = new QTreeWidgetItem(QStringList("Forward")));
	nunchukShift->addChild(m_wiimoteButtonItems[29] = new QTreeWidgetItem(QStringList("Right")));
	nunchukShift->addChild(m_wiimoteButtonItems[30] = new QTreeWidgetItem(QStringList("Left")));
	nunchukShift->addChild(m_wiimoteButtonItems[31] = new QTreeWidgetItem(QStringList("Down")));
	nunchukShift->addChild(m_wiimoteButtonItems[32] = new QTreeWidgetItem(QStringList("Up")));
	nunchukTilt->addChild(m_wiimoteButtonItems[33] = new QTreeWidgetItem(QStringList("Front")));
	nunchukTilt->addChild(m_wiimoteButtonItems[34] = new QTreeWidgetItem(QStringList("Back")));
	nunchukTilt->addChild(m_wiimoteButtonItems[35] = new QTreeWidgetItem(QStringList("Right")));
	nunchukTilt->addChild(m_wiimoteButtonItems[36] = new QTreeWidgetItem(QStringList("Left")));
	nunchukButtons->addChild(nunchukStick);
	nunchukButtons->addChild(nunchukShift);
	nunchukButtons->addChild(nunchukTilt);
	nunchuk->addChild(nunchukButtons);
	m_stickItems[0] = new QTreeWidgetItem();
	m_stickItems[0]->setText(0, "Stick");
	nunchuk->addChild(m_stickItems[0]);
	QTreeWidgetItem *classic = new QTreeWidgetItem();
	classic->setText(0, "Classic Controller");
	QTreeWidgetItem *classicButtons = new QTreeWidgetItem();
	classicButtons->setText(0, "Buttons");
	QTreeWidgetItem *classicDPad = new QTreeWidgetItem(QStringList("DPad"));
	QTreeWidgetItem *classicLStick = new QTreeWidgetItem(QStringList("Stick [L]"));
	QTreeWidgetItem *classicRStick = new QTreeWidgetItem(QStringList("Stick [R]"));
	classicButtons->addChild(m_wiimoteButtonItems[37] = new QTreeWidgetItem(QStringList("X")));
	classicButtons->addChild(m_wiimoteButtonItems[38] = new QTreeWidgetItem(QStringList("Y")));
	classicButtons->addChild(m_wiimoteButtonItems[39] = new QTreeWidgetItem(QStringList("A")));
	classicButtons->addChild(m_wiimoteButtonItems[40] = new QTreeWidgetItem(QStringList("B")));
	classicButtons->addChild(m_wiimoteButtonItems[41] = new QTreeWidgetItem(QStringList("L")));
	classicButtons->addChild(m_wiimoteButtonItems[42] = new QTreeWidgetItem(QStringList("R")));
	classicButtons->addChild(m_wiimoteButtonItems[43] = new QTreeWidgetItem(QStringList("ZL")));
	classicButtons->addChild(m_wiimoteButtonItems[44] = new QTreeWidgetItem(QStringList("ZR")));
	classicButtons->addChild(m_wiimoteButtonItems[45] = new QTreeWidgetItem(QStringList("Minus")));
	classicButtons->addChild(m_wiimoteButtonItems[46] = new QTreeWidgetItem(QStringList("Plus")));
	classicButtons->addChild(m_wiimoteButtonItems[47] = new QTreeWidgetItem(QStringList("Home")));
	classicDPad->addChild(m_wiimoteButtonItems[48] = new QTreeWidgetItem(QStringList("Right")));
	classicDPad->addChild(m_wiimoteButtonItems[49] = new QTreeWidgetItem(QStringList("Left")));
	classicDPad->addChild(m_wiimoteButtonItems[50] = new QTreeWidgetItem(QStringList("Down")));
	classicDPad->addChild(m_wiimoteButtonItems[51] = new QTreeWidgetItem(QStringList("Up")));
	classicLStick->addChild(m_wiimoteButtonItems[52] = new QTreeWidgetItem(QStringList("Right")));
	classicLStick->addChild(m_wiimoteButtonItems[53] = new QTreeWidgetItem(QStringList("Left")));
	classicLStick->addChild(m_wiimoteButtonItems[54] = new QTreeWidgetItem(QStringList("Down")));
	classicLStick->addChild(m_wiimoteButtonItems[55] = new QTreeWidgetItem(QStringList("Up")));
	classicRStick->addChild(m_wiimoteButtonItems[56] = new QTreeWidgetItem(QStringList("Right")));
	classicRStick->addChild(m_wiimoteButtonItems[57] = new QTreeWidgetItem(QStringList("Left")));
	classicRStick->addChild(m_wiimoteButtonItems[58] = new QTreeWidgetItem(QStringList("Down")));
	classicRStick->addChild(m_wiimoteButtonItems[59] = new QTreeWidgetItem(QStringList("Up")));
	classicButtons->addChild(classicDPad);
	classicButtons->addChild(classicLStick);
	classicButtons->addChild(classicRStick);
	classic->addChild(classicButtons);
	m_stickItems[1] = new QTreeWidgetItem();
	m_stickItems[1]->setText(0, "Stick [L]");
	classic->addChild(m_stickItems[1]);
	m_stickItems[2] = new QTreeWidgetItem();
	m_stickItems[2]->setText(0, "Stick [R]");
	classic->addChild(m_stickItems[2]);
	opts.last()->addChild(classic);
	opts << new QTreeWidgetItem();
	opts.last()->setText(0, "Status");
	opts.last()->addChild(m_wiimoteStatusItems[0] = new QTreeWidgetItem(QStringList("Current Latency")));
	opts.last()->addChild(m_wiimoteStatusItems[1] = new QTreeWidgetItem(QStringList("Average Latency")));
	opts.last()->addChild(m_wiimoteStatusItems[2] = new QTreeWidgetItem(QStringList("Battery")));
	opts.last()->addChild(m_wiimoteStatusItems[3] = new QTreeWidgetItem(QStringList("Leds")));
	opts.last()->addChild(m_wiimoteStatusItems[4] = new QTreeWidgetItem(QStringList("Rumble")));
	opts.last()->addChild(m_wiimoteStatusItems[5] = new QTreeWidgetItem(QStringList("Extension")));
	m_wiimoteStatusItems[3]->addChild(m_wiimoteLedItems[0] = new QTreeWidgetItem(QStringList("Led [1]")));
	m_wiimoteStatusItems[3]->addChild(m_wiimoteLedItems[1] = new QTreeWidgetItem(QStringList("Led [2]")));
	m_wiimoteStatusItems[3]->addChild(m_wiimoteLedItems[2] = new QTreeWidgetItem(QStringList("Led [3]")));
	m_wiimoteStatusItems[3]->addChild(m_wiimoteLedItems[3] = new QTreeWidgetItem(QStringList("Led [4]")));
	ui->centralwidget->layout()->addWidget(graphics);
	ui->treeWidget->header()->setStretchLastSection(false);
	//  ui->treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
	//  ui->treeWidget->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	//  ui->treeWidget->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	//  ui->treeWidget->header()->setResizeMode(3, QHeaderView::ResizeToContents);
	ui->treeWidget->insertTopLevelItems(0, opts);
	label->setMargin(2);
	connect(m_wiimotePoints[0], SIGNAL(triggered()), this, SLOT(pressedPoint1()));
	connect(m_wiimotePoints[1], SIGNAL(triggered()), this, SLOT(pressedPoint2()));
	connect(m_wiimotePoints[2], SIGNAL(triggered()), this, SLOT(pressedPoint3()));
	connect(m_wiimotePoints[3], SIGNAL(triggered()), this, SLOT(pressedPoint4()));
	connect(m_classicConnected, SIGNAL(triggered()), this, SLOT(pressedClassic()));
	connect(m_nunchukConnected, SIGNAL(triggered()), this, SLOT(pressedNunchuk()));
	connect(m_cursorVisible, SIGNAL(triggered()), this, SLOT(pressedCursor()));
	connect(m_wiimoteLeds[0], SIGNAL(triggered()), this, SLOT(toggleLed1()));
	connect(m_wiimoteLeds[1], SIGNAL(triggered()), this, SLOT(toggleLed2()));
	connect(m_wiimoteLeds[2], SIGNAL(triggered()), this, SLOT(toggleLed3()));
	connect(m_wiimoteLeds[3], SIGNAL(triggered()), this, SLOT(toggleLed4()));
	connect(m_wiimoteRumble, SIGNAL(triggered()), this, SLOT(toggleRumble()));
	QList<uint> ids = m_interface->dbusGetWiimoteList().value();

	if (ids.isEmpty())
		changeWiimote(0);
	else
		changeWiimote(ids.first());

	startTimer(1000);
	connect(m_wiimoteComboBox, SIGNAL(activated(int)), this, SLOT(wiimoteComboBoxChanged(int)));
}

void WiimoteRawStream::wiimoteComboBoxChanged(int index) {
	changeWiimote(m_wiimoteComboBox->itemData(index).toUInt());
}

void WiimoteRawStream::pressedPoint1() {
	ui->treeWidget->expandItem(opts.at(2));
	m_infraredItems[0]->setSelected(!m_infraredItems[0]->isSelected());
}

void WiimoteRawStream::pressedPoint2() {
	ui->treeWidget->expandItem(opts.at(2));
	m_infraredItems[1]->setSelected(!m_infraredItems[1]->isSelected());
}

void WiimoteRawStream::pressedPoint3() {
	ui->treeWidget->expandItem(opts.at(2));
	m_infraredItems[2]->setSelected(!m_infraredItems[2]->isSelected());
}

void WiimoteRawStream::pressedPoint4() {
	ui->treeWidget->expandItem(opts.at(2));
	m_infraredItems[3]->setSelected(!m_infraredItems[3]->isSelected());
}

void WiimoteRawStream::pressedClassic() {
	ui->treeWidget->expandItem(opts.at(3));
	opts[3]->child(1)->setExpanded(!opts[3]->child(1)->isExpanded());
}

void WiimoteRawStream::pressedNunchuk() {
	ui->treeWidget->expandItem(opts.at(3));
	opts[3]->child(0)->setExpanded(!opts[3]->child(0)->isExpanded());
}

void WiimoteRawStream::pressedCursor() {
	ui->treeWidget->expandItem(opts.at(2));
	opts[2]->child(4)->setExpanded(!opts[2]->child(4)->isExpanded());
}

void WiimoteRawStream::updateWiimoteComboBox() {
	m_wiimoteComboBox->clear();
	foreach (const uint &id, m_interface->dbusGetWiimoteList().value()) {
		m_wiimoteComboBox->addItem("[" + QString::number(id) + "]-" + m_interface->dbusWiimoteGetMacAddress(id).value(), id);
		m_wiimoteComboBox->setItemData(m_wiimoteComboBox->count() - 1, id);
	}

	for (int i = 0; i < m_wiimoteComboBox->count(); ++i) {
		if (m_wiimoteComboBox->itemData(i).toUInt() == m_id) {
			m_wiimoteComboBox->setCurrentIndex(i);
			break;
		}
	}
}

void WiimoteRawStream::changeWiimote(uint id) {
	m_id = id;
	m_mainWindow->setWiimoteId(id);

	for (int i = 0; i < 60; ++i) {
		m_wiimoteButtonItems[i]->setText(1, "[ ]");
		m_wiimoteButtonItems[i]->setData(0, Qt::UserRole, false);
	}

	m_wiimoteStatusItems[0]->setText(1, "0ms");
	m_wiimoteStatusItems[1]->setText(1, "0ms");
	m_wiimoteStatusItems[2]->setText(1, "0%");
	updateWiimoteComboBox();
	accdata acc;
	memset(&acc, 0, sizeof(acc));
	dbusWiimoteInfrared(m_id, QList<irpoint>());
	dbusWiimoteAcc(m_id, acc);
	dbusWiimoteLedStatusChanged(m_id, m_interface->dbusWiimoteGetLedStatus(m_id).value());
	dbusWiimoteRumbleStatusChanged(m_id, m_interface->dbusWiimoteGetRumbleStatus(m_id).value());
	dbusVirtualCursorPosition(m_id, 0, 0, 0, 0);
	dbusVirtualCursorLost(m_id);

	if (m_interface->dbusIsClassicConnected(m_id).value())
		dbusClassicPlugged(m_id);
	else
		dbusClassicUnplugged(m_id);

	if (m_interface->dbusIsNunchukConnected(m_id).value())
		dbusNunchukPlugged(m_id);
	else
		dbusNunchukUnplugged(m_id);

	dbusWiimoteGeneralButtons(m_id, 0);
	m_wiimoteBatteryProgressBar->setMaximum(100);
	m_wiimoteBatteryProgressBar->setMinimum(0);
	m_wiimoteBatteryProgressBar->setValue(m_interface->dbusWiimoteGetBatteryLife(m_id).value());
}

void WiimoteRawStream::timerEvent(QTimerEvent *event) {
	if (!m_interface->dbusIsWiimoteConnected(m_id).value())
		return;

	m_wiimoteStatusItems[0]->setText(1, QString::number(m_interface->dbusWiimoteGetCurrentLatency(m_id).value()) + "ms");
	m_wiimoteStatusItems[1]->setText(1, QString::number(m_interface->dbusWiimoteGetAverageLatency(m_id).value()) + "ms");
	m_wiimoteStatusItems[2]->setText(1, QString::number(m_interface->dbusWiimoteGetBatteryLife(m_id).value()) + "%");
	m_wiimoteBatteryProgressBar->setMaximum(100);
	m_wiimoteBatteryProgressBar->setMinimum(0);
	m_wiimoteBatteryProgressBar->setValue(m_interface->dbusWiimoteGetBatteryLife(m_id).value());
}

void WiimoteRawStream::toggleRumble() {
	bool status = m_interface->dbusWiimoteGetRumbleStatus(m_id).value();

	if (status)
		m_interface->dbusWiimoteSetRumbleStatus(m_id, false);
	else
		m_interface->dbusWiimoteSetRumbleStatus(m_id, true);
}

void WiimoteRawStream::toggleLed1() {
	quint8 status = m_interface->dbusWiimoteGetLedStatus(m_id).value();

	if (status & 0b0001)
		m_interface->dbusWiimoteSetLedStatus(m_id, status & 0b1110);
	else
		m_interface->dbusWiimoteSetLedStatus(m_id, status | 0b0001);
}

void WiimoteRawStream::toggleLed2() {
	quint8 status = m_interface->dbusWiimoteGetLedStatus(m_id).value();

	if (status & 0b0010)
		m_interface->dbusWiimoteSetLedStatus(m_id, status & 0b1101);
	else
		m_interface->dbusWiimoteSetLedStatus(m_id, status | 0b0010);
}

void WiimoteRawStream::toggleLed3() {
	quint8 status = m_interface->dbusWiimoteGetLedStatus(m_id).value();

	if (status & 0b0100)
		m_interface->dbusWiimoteSetLedStatus(m_id, status & 0b1011);
	else
		m_interface->dbusWiimoteSetLedStatus(m_id, status | 0b0100);
}

void WiimoteRawStream::toggleLed4() {
	quint8 status = m_interface->dbusWiimoteGetLedStatus(m_id).value();

	if (status & 0b1000)
		m_interface->dbusWiimoteSetLedStatus(m_id, status & 0b0111);
	else
		m_interface->dbusWiimoteSetLedStatus(m_id, status | 0b1000);
}

void WiimoteRawStream::dbusWiimoteConnected(uint id) {
	updateWiimoteComboBox();

	if (m_id == 0)
		m_id = id;
	else {
		if (m_id != id)
			return;
	}

	changeWiimote(m_id);

	if (m_interface->dbusIsNunchukConnected(id))
		m_wiimoteStatusItems[5]->setText(1, "nunchuk");
	else if (m_interface->dbusIsClassicConnected(id))
		m_wiimoteStatusItems[5]->setText(1, "classic controller");
	else
		m_wiimoteStatusItems[5]->setText(1, "none");
}

void WiimoteRawStream::dbusWiimoteDisconnected(uint id) {
	updateWiimoteComboBox();

	if (m_id != id)
		return;

	m_wiimoteStatusItems[5]->setText(1, "none");
	QList<uint> ids = m_interface->dbusGetWiimoteList().value();

	if (ids.isEmpty())
		changeWiimote(0);
	else
		changeWiimote(ids.first());
}

void WiimoteRawStream::dbusNunchukPlugged(uint id) {
	if (m_id != id)
		return;

	m_wiimoteStatusItems[5]->setText(1, "nunchuk");
	m_nunchukConnected->setText("Nunchuk[*]");
	m_nunchukConnected->setIcon(QIcon(":/nunchuk_active.png"));
}

void WiimoteRawStream::dbusNunchukUnplugged(uint id) {
	if (m_id != id)
		return;

	m_wiimoteStatusItems[5]->setText(1, "none");
	m_nunchukConnected->setText("Nunchuk[ ]");
	m_nunchukConnected->setIcon(QIcon(":/nunchuk.png"));
	accdata acc;
	memset(&acc, 0, sizeof(acc));
	stickdata stick;
	memset(&stick, 0, sizeof(stick));
	dbusNunchukStick(m_id, stick);
	dbusNunchukAcc(m_id, acc);
}

void WiimoteRawStream::dbusClassicPlugged(uint id) {
	if (m_id != id)
		return;

	m_wiimoteStatusItems[5]->setText(1, "classic controller");
	m_classicConnected->setText("Classic[*]");
	m_classicConnected->setIcon(QIcon(":/controller_active.png"));
}

void WiimoteRawStream::dbusClassicUnplugged(uint id) {
	if (m_id != id)
		return;

	m_wiimoteStatusItems[5]->setText(1, "none");
	m_classicConnected->setText("Classic[ ]");
	m_classicConnected->setIcon(QIcon(":/controller.png"));
	stickdata stick;
	memset(&stick, 0, sizeof(stick));
	dbusClassicControllerLStick(m_id, stick);
	dbusClassicControllerRStick(m_id, stick);
}

void WiimoteRawStream::dbusVirtualCursorPosition(uint id, double x, double y, double size, double angle) {
	if (m_id != id)
		return;

	ui->treeWidget->setUpdatesEnabled(false);
	m_infraredItems[5]->setText(1, QString::number(int(x)) + "x" + QString::number(int(y)));
	m_infraredItems[6]->setText(1, QString::number(angle));
	m_infraredItems[7]->setText(1, QString::number(size));
	ui->treeWidget->setUpdatesEnabled(true);
}

void WiimoteRawStream::dbusVirtualCursorLost(uint id) {
	if (m_id != id)
		return;

	m_infraredItems[4]->setText(1, "false");
	m_cursorVisible->setIcon(QIcon(":/cursor_disabled.png"));
	m_cursorVisible->setText("Cursor[ ]");
	dbusWiimoteInfrared(m_id, QList<irpoint>());
}

void WiimoteRawStream::dbusVirtualCursorFound(uint id) {
	if (m_id != id)
		return;

	m_infraredItems[4]->setText(1, "true");
	m_cursorVisible->setIcon(QIcon(":/cursor.png"));
	m_cursorVisible->setText("Cursor[*]");
}

void WiimoteRawStream::dbusWiimoteAcc(uint id, const accdata &acc) {
	if (m_id != id)
		return;

	ui->treeWidget->setUpdatesEnabled(false);
	m_accelerometerItems[0][0]->setText(1, QString::number(acc.x));
	m_accelerometerItems[0][1]->setText(1, QString::number(acc.y));
	m_accelerometerItems[0][2]->setText(1, QString::number(acc.z));
	m_accelerometerItems[0][3]->setText(1, QString::number(int(acc.pitch)) + QString::fromUtf8("°"));
	m_accelerometerItems[0][4]->setText(1, QString::number(int(acc.roll)) + QString::fromUtf8("°"));
	ui->treeWidget->setUpdatesEnabled(true);
}

void WiimoteRawStream::dbusNunchukAcc(uint id, const accdata &acc) {
	if (m_id != id)
		return;

	ui->treeWidget->setUpdatesEnabled(false);
	m_accelerometerItems[1][0]->setText(1, QString::number(acc.x));
	m_accelerometerItems[1][1]->setText(1, QString::number(acc.y));
	m_accelerometerItems[1][2]->setText(1, QString::number(acc.z));
	m_accelerometerItems[1][3]->setText(1, QString::number(acc.pitch) + QString::fromUtf8("⁰"));
	m_accelerometerItems[1][4]->setText(1, QString::number(acc.roll) + QString::fromUtf8("⁰"));
	ui->treeWidget->setUpdatesEnabled(true);
}

void WiimoteRawStream::dbusWiimoteBatteryLife(uint id, uint8 life) {
	if (m_id != id)
		return;

	m_wiimoteBatteryProgressBar->setMaximum(100);
	m_wiimoteBatteryProgressBar->setMinimum(0);
	m_wiimoteBatteryProgressBar->setValue(life);
	m_wiimoteStatusItems[2]->setText(1, QString::number(life) + "%");
}

void WiimoteRawStream::dbusWiimoteGeneralButtons(uint id, uint64 value) {
	if (m_id != id)
		return;

	ui->treeWidget->setUpdatesEnabled(false);

	for (int i = 0; i < 60; ++i) {
		if (value & (uint64(1) << i)) {
			if (!m_wiimoteButtonItems[i]->data(0, Qt::UserRole).toBool()) {
				m_wiimoteButtonItems[i]->setText(1, "[*]");
				m_wiimoteButtonItems[i]->setData(0, Qt::UserRole, true);
			}
		} else {
			if (m_wiimoteButtonItems[i]->data(0, Qt::UserRole).toBool()) {
				m_wiimoteButtonItems[i]->setText(1, "[ ]");
				m_wiimoteButtonItems[i]->setData(0, Qt::UserRole, false);
			}
		}
	}

	ui->treeWidget->setUpdatesEnabled(true);
}

void WiimoteRawStream::dbusWiimoteInfrared(uint id, const QList<struct irpoint> &points) {
	if (m_id != id)
		return;

	ui->treeWidget->setUpdatesEnabled(false);
	register int i = 0;
	QString str;

	for (; i < points.count(); ++i) {
		str = QString::number(points[i].x) + "x" + QString::number(points[i].y) + ", size: " + QString::number(points[i].size);

		if (m_infraredItems[i]->text(1) != str)
			m_infraredItems[i]->setText(1, str);

		if (!m_wiimotePoints[i]->data().toBool()) {
			m_wiimotePoints[i]->setIcon(QIcon(":/enabled24.png"));
			m_wiimotePoints[i]->setData(true);
		}
	}

	str = "0x0, size: -1";

	for (; i < 4; ++i) {
		if (m_infraredItems[i]->text(1) != str)
			m_infraredItems[i]->setText(1, str);

		if (m_wiimotePoints[i]->data().toBool()) {
			m_wiimotePoints[i]->setIcon(QIcon(":/disabled24.png"));
			m_wiimotePoints[i]->setData(false);
		}
	}

	ui->treeWidget->setUpdatesEnabled(true);
}

void WiimoteRawStream::dbusWiimoteLedStatusChanged(uint id, uint8 status) {
	if (m_id != id)
		return;

	for (int i = 0; i < 4; ++i)
		if (status & (1 << i)) {
			m_wiimoteLeds[i]->setIcon(QIcon(":/enabled_blue24.png"));
			m_wiimoteLedItems[i]->setText(1, "on");
		} else {
			m_wiimoteLeds[i]->setIcon(QIcon(":/disabled24.png"));
			m_wiimoteLedItems[i]->setText(1, "off");
		}
}

void WiimoteRawStream::dbusWiimoteRumbleStatusChanged(uint id, uint8 status) {
	if (m_id != id)
		return;

	if (status) {
		m_wiimoteRumble->setText("Rumble[*]");
		m_wiimoteRumble->setIcon(QIcon(":/rumble_on.png"));
		m_wiimoteStatusItems[4]->setText(1, "on");
	} else {
		m_wiimoteRumble->setText("Rumble[ ]");
		m_wiimoteRumble->setIcon(QIcon(":/rumble_off.png"));
		m_wiimoteStatusItems[4]->setText(1, "off");
	}
}

void WiimoteRawStream::dbusNunchukStick(uint id, const stickdata &stick) {
	if (m_id != id)
		return;

	m_stickItems[0]->setText(1, QString::number(stick.x) + "x" + QString::number(stick.y));
}

void WiimoteRawStream::dbusClassicControllerLStick(uint id, const stickdata &stick) {
	if (m_id != id)
		return;

	m_stickItems[1]->setText(1, QString::number(stick.x) + "x" + QString::number(stick.y));
}

void WiimoteRawStream::dbusClassicControllerRStick(uint id, const stickdata &stick) {
	if (m_id != id)
		return;

	m_stickItems[2]->setText(1, QString::number(stick.x) + "x" + QString::number(stick.y));
}

WiimoteRawStream::~WiimoteRawStream() {
	delete ui;
}
