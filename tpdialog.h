// created by Dr. Weiss

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QImage>
#include <QSpinBox>
#include <QString>
#include <QWidget>
#include "tiepoint.h"

class TPDialog: public QDialog
{
        Q_OBJECT;
    public:
        TPDialog( QImage img, QWidget *parent = NULL );
        TPDialog( QImage img, uint width, uint height, QWidget *parent = NULL );
        TPDialog( QImage img, QString title, QWidget *parent = NULL );
        TPDialog( QImage img, QString title, uint width, uint height,
                  QWidget *parent = NULL );
        virtual ~TPDialog();
        TiePoints *Source();
        TiePoints *Target();
    private:
        void init();
        void initTop();
        void initBottom();
    private:
        QGridLayout *layout;
        QSpinBox *spnRow, *spnCol;
        TiePoints *tpSource, *tpTarget;
        QDialogButtonBox *btnBox;
};
