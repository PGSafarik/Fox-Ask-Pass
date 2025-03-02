#include <iostream>
#include <unistd.h>

#include <fox-1.7/fx.h>

#include "icons.h"

/**********************************************************************************************************************/
class  MainWindow : public FXMainWindow {
FXDECLARE( MainWindow )
  // Window
  FXLabel *m_title_lbl;

  // Input system
  FXSwitcher  *m_switch_box;
  FXLabel     *m_input_lbl;
  FXTextField *m_input_tf;

  // Controls
  FXButton    *m_accept_btn;
  FXButton    *m_close_btn;

  // Icons
  FXIcon *accept_ic;
  FXIcon *cancel_ic;
  FXIcon *clear_ic;
  FXIcon *locks_ic;
  FXIcon *lockb_ic;
  FXIcon *show_ic;

public:
  MainWindow( FXApp *application );
  virtual ~MainWindow();

  /* Operation methods */
  void create( );
  void check_calller( );

  /* Gui events and handlers */
  enum {
    ID_ACCEPT = FXMainWindow::ID_LAST,
    ID_STORNO,
    ID_CLEAR,
    ID_VIEW,
    ID_LAST
  };
  long onCmd_Execute( FXObject *tgt, FXSelector sel, void *data );
  long onCmd_Field( FXObject *tgt, FXSelector sel, void *data );

protected:
  MainWindow() { }

  /* helpest methods */
  FXString ReadProcStat( FXString pid, FXint index );
  void     UpdateFocus( );
  void     ShortCuts( );
};
/**********************************************************************************************************************/
int main( int argc, char **argv ) {
  FXApp app;
  app.init( argc, argv );

  new MainWindow( &app );
  app.create( );

  return app.run( );
}

/**********************************************************************************************************************/
FXDEFMAP( MainWindow ) MW_MAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, MainWindow::ID_ACCEPT, MainWindow::onCmd_Execute ),
  FXMAPFUNC( SEL_COMMAND, MainWindow::ID_STORNO, MainWindow::onCmd_Execute ),
  FXMAPFUNC( SEL_COMMAND, MainWindow::ID_CLEAR, MainWindow::onCmd_Field ),
  FXMAPFUNC( SEL_LEFTBUTTONPRESS,   MainWindow::ID_VIEW, MainWindow::onCmd_Field ),
  FXMAPFUNC( SEL_LEFTBUTTONRELEASE, MainWindow::ID_VIEW, MainWindow::onCmd_Field ),
};
FXIMPLEMENT( MainWindow, FXMainWindow, MW_MAP, ARRAYNUMBER( MW_MAP ) )

/**********************************************************************************************************************/
MainWindow::MainWindow( FXApp *application )
          : FXMainWindow( application, "FOX Ask Password Client", nullptr, nullptr, DECOR_TITLE | DECOR_BORDER | DECOR_MENU | DECOR_CLOSE, 0, 0, 380, 120 )
{
  /* Make icons */
  accept_ic =  new FXPNGIcon( getApp( ), accept_icdata,    0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS );
  cancel_ic =  new FXPNGIcon( getApp( ), cancel_icdata,    0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS | IMAGE_DITHER);
  clear_ic  =  new FXPNGIcon( getApp( ), clear_icdata,     0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS );
  lockb_ic  =  new FXPNGIcon( getApp( ), lockbig_icdata,   0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS | IMAGE_DITHER );
  locks_ic  =  new FXPNGIcon( getApp( ), locksmall_icdata, 0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS );
  show_ic   =  new FXPNGIcon( getApp( ), showtext_icdata,  0, IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS );

  /* Window mask */
  FXVerticalFrame   *content = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_SIDE_TOP | LAYOUT_FILL_X );
  FXHorizontalFrame *title_box   = new FXHorizontalFrame( content, FRAME_NONE | LAYOUT_FILL_X );
  FXHorizontalFrame *input_box   = new FXHorizontalFrame( content, FRAME_LINE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0,   0, 0 );
  FXHorizontalFrame *control_box = new FXHorizontalFrame( this,  FRAME_NONE | LAYOUT_FILL_X );

  /* Title box */
  m_title_lbl = new FXLabel( title_box, "Enter Your user password, please:", lockb_ic, LABEL_NORMAL | LAYOUT_FILL );

  /* Input Box */
  m_switch_box = new FXSwitcher( input_box, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
  m_input_tf   = new FXTextField( m_switch_box, 120, this, ID_ACCEPT, TEXTFIELD_PASSWD | JUSTIFY_CENTER_X | LAYOUT_FILL_X );
  m_input_lbl  = new FXLabel( m_switch_box, FXString::null, nullptr, 0, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FILL_X );
  m_input_lbl->setBackColor( FXRGB( 255, 255, 255 ) );
  m_input_lbl->setJustify( JUSTIFY_LEFT );

  new FXButton( input_box, "\t\t clear", clear_ic, this, ID_CLEAR, BUTTON_TOOLBAR );
  new FXButton( input_box, "\t\t show",  show_ic,  this, ID_VIEW, BUTTON_TOOLBAR );

  /* Control Box */
  m_close_btn  = new FXButton( control_box, "Close",  cancel_ic, this, ID_STORNO, BUTTON_NORMAL | LAYOUT_RIGHT  );
  m_accept_btn = new FXButton( control_box, "Accept", accept_ic, this, ID_ACCEPT, BUTTON_NORMAL | LAYOUT_RIGHT  );

  this->setIcon( lockb_ic );
  this->setMiniIcon( locks_ic );

  check_calller( );
  ShortCuts( );
}

MainWindow::~MainWindow( )
{ }

void MainWindow::create( )
{
  FXMainWindow::create( );
  accept_ic->create( );
  cancel_ic->create( );
  clear_ic->create( );
  locks_ic->create( );
  lockb_ic->create( );
  show_ic->create( );

  show( PLACEMENT_SCREEN );
  UpdateFocus( );
}

void MainWindow::check_calller( )
{
  FXString p_pid  = FXString::value( getppid( ) );
  FXString p_name = this->ReadProcStat( p_pid, 2 );

  m_title_lbl->setText( p_pid + " " + p_name + ": " + m_title_lbl->getText( ) );
}


long MainWindow::onCmd_Execute( FXObject *tgt, FXSelector sel, void *data )
{
  FXint state = 0;

  if( FXSELID( sel ) == ID_ACCEPT ) {
    FXString cl_name = tgt->getClassName( );
    if( cl_name == "FXTextField" && !m_input_tf->hasFocus( ) ) { return 0; }

    FXString inp_str = m_input_tf->getText( );
    if( !inp_str.empty( ) ) {
      std::cout << inp_str.text( ) << std::endl;
      state = 1;
    }
  }

  getApp( )->exit( state );

  return 0;
}

long MainWindow::onCmd_Field( FXObject *tgt, FXSelector sel, void *data )
{
  switch( FXSELID( sel ) ) {
    case ID_CLEAR :
    {
      if( FXMessageBox::warning( this, MBOX_YES_NO ,"Clear input", "Really clear the password?" ) == MBOX_CLICKED_YES  ) {
        m_input_tf->clearText( );
      }
      break;
    }
    case ID_VIEW :
    {
      FXuint type = FXSELTYPE( sel );
      if( type == SEL_LEFTBUTTONPRESS ) {
        m_input_lbl->setText( m_input_tf->getText( ) );
        m_switch_box->setCurrent( 1 );
      }
      else if( type == SEL_LEFTBUTTONRELEASE ) {
        m_switch_box->setCurrent( 0 );
        m_input_lbl->setText( FXString::null );
      }

      break;
    }
  }

  UpdateFocus( );
  return 0;
}

/**********************************************************************************************************************/
FXString MainWindow::ReadProcStat( FXString pid, FXint index )
{
  FXString data = FXString::null;
  FXint start, count = 0;
  FXchar ch;

  FXString filename = "/proc/";
  filename += pid + "/stat";

  if( FXStat::exists( filename ) ) {
    FXFile fd;
    if ( fd.open( filename, FXIO::Reading, FXIO::AllRead ) ) {
      start = index - 1;
      while ( count != index  ) {
        fd.readChar( ch );
        if ( ch == ' ' ) { count++; continue; }
        else if ( count == start) {
          data += ch;
        }
      }
      fd.close( );
    }
    else { std::cerr << "MainWindow::ReadProcStat: File  " << filename.text( ) << "not opened" << std::endl; }
  }
  else { std::cerr << "MainWindow::ReadProcStat: File " << filename.text( ) << " not found!" << std::endl; }

  return data;
}

void MainWindow::UpdateFocus( )
{
  if( !m_input_tf->hasFocus( ) ) {
    m_input_tf->setFocus( );
  }
}


void MainWindow::ShortCuts( )
{
  FXAccelTable *acc = getShell( )->getAccelTable( );
  if( acc ) {
    /*MKUINT( KEY_a, ALTMASK )*/
    acc->addAccel( "Alt+s" , this, FXSEL( SEL_COMMAND, ID_ACCEPT ) );
    acc->addAccel( "Esc",    this, FXSEL( SEL_COMMAND, ID_CLOSE ) );
    acc->addAccel( "Alt+c" , this, FXSEL( SEL_COMMAND, ID_CLEAR ) );
    acc->addAccel( "Alt+v" , this, FXSEL( SEL_LEFTBUTTONPRESS, ID_VIEW ), FXSEL( SEL_LEFTBUTTONRELEASE, ID_VIEW ) );
  }
}

/*** END **************************************************************************************************************/




