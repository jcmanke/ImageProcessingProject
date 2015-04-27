// created by Dr. Weiss


#include <QBoxLayout>
#include <QLabel>
#include "tpdialog.h"

TPDialog::TPDialog( QImage img, QWidget *parent ): QDialog( parent )
{
    setWindowTitle( tr( "Enter Tie Points" ) );
    tpSource = new TiePoints( img, this );
    tpTarget = new TiePoints( img, this );
    init();
}

TPDialog::TPDialog( QImage img, uint width, uint height, QWidget *parent ):
        QDialog( parent )
{
    setWindowTitle( tr( "Enter Tie Points" ) );
    tpSource = new TiePoints( img, width, height, this );
    tpTarget = new TiePoints( img, width, height, this );
    init();
}

TPDialog::TPDialog( QImage img, QString title, QWidget *parent ):
        QDialog( parent )
{
    setWindowTitle( title );
    tpSource = new TiePoints( img, this );
    tpTarget = new TiePoints( img, this );
    init();
}

TPDialog::TPDialog( QImage img, QString title, uint width, uint height,
                    QWidget *parent ): QDialog( parent )
{
    setWindowTitle( title );
    tpSource = new TiePoints( img, width, height, this );
    tpTarget = new TiePoints( img, width, height, this );
    init();
}

TPDialog::~TPDialog() {}

TiePoints *TPDialog::Source()
{
    return tpSource;
}

TiePoints *TPDialog::Target()
{
    return tpTarget;
}

void TPDialog::init()
{
    layout = new QGridLayout( this );
    initTop();
    initBottom();
    btnBox = new QDialogButtonBox( QDialogButtonBox::Ok |
                                   QDialogButtonBox::Cancel, Qt::Horizontal, this );
    layout->addWidget( btnBox, 3, 0, 1, -1 );
    connect( btnBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( btnBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
    connect( spnRow, SIGNAL( valueChanged( int ) ), tpSource, SLOT( SetRows( int ) ) );
    connect( spnCol, SIGNAL( valueChanged( int ) ), tpSource, SLOT( SetCols( int ) ) );
    connect( spnRow, SIGNAL( valueChanged( int ) ), tpTarget, SLOT( SetRows( int ) ) );
    connect( spnCol, SIGNAL( valueChanged( int ) ), tpTarget, SLOT( SetCols( int ) ) );
    setLayout( layout );
}

void TPDialog::initTop()
{
    QLabel *lblSrc = new QLabel( tr( "Source:" ), this );
    QLabel *lblTarget = new QLabel( tr( "Target:" ), this );
    lblSrc->setAlignment( Qt::AlignHCenter );
    lblTarget->setAlignment( Qt::AlignHCenter );
    layout->addWidget( lblSrc, 0, 0 );
    layout->addWidget( lblTarget, 0, 1 );
    layout->addWidget( tpSource, 1, 0 );
    layout->addWidget( tpTarget, 1, 1 );
}

void TPDialog::initBottom()
{
    QBoxLayout *box = new QBoxLayout( QBoxLayout::LeftToRight );
    QLabel *lblRow = new QLabel( tr( "Rows:" ), this );
    QLabel *lblCol = new QLabel( tr( "Columns:" ), this );
    spnRow = new QSpinBox( this );
    spnCol = new QSpinBox( this );
    spnRow->setValue( tpSource->Rows() );
    spnCol->setValue( tpSource->Cols() );
    spnRow->setMinimum( 2 );
    spnRow->setMaximum( 20 );
    spnCol->setMinimum( 2 );
    spnCol->setMaximum( 20 );
    layout->addLayout( box, 2, 0 );
    box->addWidget( lblRow );
    box->addWidget( spnRow );
    box->addWidget( lblCol );
    box->addWidget( spnCol );
}
