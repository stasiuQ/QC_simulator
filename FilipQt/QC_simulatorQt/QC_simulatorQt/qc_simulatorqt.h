#pragma once

#include <QtWidgets/QWidget>
#include "ui_qc_simulatorqt.h"

class QC_simulatorQt : public QWidget
{
	Q_OBJECT

public:
	QC_simulatorQt(QWidget *parent = Q_NULLPTR);

private:
	Ui::QC_simulatorQtClass ui;
};
