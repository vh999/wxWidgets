///////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/window.h
// Purpose:     wxWindow class
// Author:      Peter Most, Javier Torres, Mariano Reingart
// Copyright:   (c) 2009 wxWidgets dev team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_WINDOW_H_
#define _WX_QT_WINDOW_H_

#include <memory>

class QShortcut;

class QAbstractScrollArea;
class QPainter;
class QPicture;
class QScrollBar;
class QWidget;

class QCloseEvent;
class QContextMenuEvent;
class QEvent;
class QFocusEvent;
class QKeyEvent;
class QPaintEvent;
class QResizeEvent;
class QWheelEvent;
class QMouseEvent;
class QMoveEvent;

class WXDLLIMPEXP_FWD_CORE wxQtShortcutHandler;

/* wxQt specific notes:
 *
 * Event handling is achieved by using the template class wxQtEventSignalHandler
 * found in winevent.h to send all Qt events here to QtHandleXXXEvent() methods.
 * All these methods receive the Qt event and the handler. This is done because
 * events of the containers (the scrolled part of the window) are sent to the
 * same wxWindow instance, that must be able to differentiate them as some events
 * need different handling (paintEvent) depending on that.
 * We pass the QWidget pointer to all event handlers for consistency.
 */
class WXDLLIMPEXP_CORE wxWindowQt : public wxWindowBase
{
public:
    wxWindowQt();
    ~wxWindowQt();
    wxWindowQt(wxWindowQt *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));

    bool Create(wxWindowQt *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));

    // Derived classes have to call PostCreation() explicitly if they don't call
    // our Create() method during widget creation process.
    void PostCreation( bool generic = true );

    void AddChild( wxWindowBase *child ) override;

    virtual bool Show( bool show = true ) override;

    virtual void SetLabel(const wxString& label) override;
    virtual wxString GetLabel() const override;

    virtual void DoEnable( bool enable ) override;
    virtual void SetFocus() override;

    // Parent/Child:
    static void QtReparent( QWidget *child, QWidget *parent );
    virtual bool Reparent( wxWindowBase *newParent ) override;

    // Z-order
    virtual void Raise() override;
    virtual void Lower() override;

    // move the mouse to the specified position
    virtual void WarpPointer(int x, int y) override;

    virtual void Update() override;
    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = nullptr ) override;

    virtual bool SetCursor( const wxCursor &cursor ) override;
    virtual bool SetFont(const wxFont& font) override;

    // get the (average) character size for the current font
    virtual int GetCharHeight() const override;
    virtual int GetCharWidth() const override;
    virtual double GetContentScaleFactor() const override;

    virtual wxSize GetDPI() const override;
    virtual double GetDPIScaleFactor() const override;

    virtual void SetScrollbar( int orient,
                               int pos,
                               int thumbvisible,
                               int range,
                               bool refresh = true ) override;
    virtual void SetScrollPos( int orient, int pos, bool refresh = true ) override;
    virtual int GetScrollPos( int orient ) const override;
    virtual int GetScrollThumb( int orient ) const override;
    virtual int GetScrollRange( int orient ) const override;

    virtual wxSize GetWindowBorderSize() const override;

        // scroll window to the specified position
    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = nullptr ) override;

    // Styles
    virtual void SetWindowStyleFlag( long style ) override;
    virtual void SetExtraStyle( long exStyle ) override;

    virtual bool SetBackgroundStyle(wxBackgroundStyle style) override;
    virtual bool IsTransparentBackgroundSupported(wxString* reason = nullptr) const override;
    virtual bool SetTransparent(wxByte alpha) override;
    virtual bool CanSetTransparent() override { return true; }

    virtual bool SetBackgroundColour(const wxColour& colour) override;
    virtual bool SetForegroundColour(const wxColour& colour) override;

    virtual void SetDoubleBuffered(bool on) override;
    virtual bool IsDoubleBuffered() const override;

    QWidget *GetHandle() const override;

#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget ) override;
#endif

#if wxUSE_ACCEL
    // accelerators
    // ------------
    virtual void SetAcceleratorTable( const wxAcceleratorTable& accel ) override;
#endif // wxUSE_ACCEL

    virtual bool EnableTouchEvents(int eventsMask) override;

    // wxQt implementation internals:

    // Takes ownership of pict - window will delete it
    void QtSetPicture( QPicture* pict );

    QPainter *QtGetPainter();
    virtual bool QtCanPaintWithoutActivePainter() const;

    virtual bool QtHandlePaintEvent  ( QWidget *handler, QPaintEvent *event );
    virtual bool QtHandleResizeEvent ( QWidget *handler, QResizeEvent *event );
    virtual bool QtHandleWheelEvent  ( QWidget *handler, QWheelEvent *event );
    virtual bool QtHandleKeyEvent    ( QWidget *handler, QKeyEvent *event );
    virtual bool QtHandleMouseEvent  ( QWidget *handler, QMouseEvent *event );
    virtual bool QtHandleEnterEvent  ( QWidget *handler, QEvent *event );
    virtual bool QtHandleMoveEvent   ( QWidget *handler, QMoveEvent *event );
    virtual bool QtHandleShowEvent   ( QWidget *handler, QEvent *event );
    virtual bool QtHandleChangeEvent ( QWidget *handler, QEvent *event );
    virtual bool QtHandleCloseEvent  ( QWidget *handler, QCloseEvent *event );
    virtual bool QtHandleContextMenuEvent  ( QWidget *handler, QContextMenuEvent *event );
    virtual bool QtHandleFocusEvent  ( QWidget *handler, QFocusEvent *event );

    static void QtStoreWindowPointer( QWidget *widget, const wxWindowQt *window );
    static wxWindowQt *QtRetrieveWindowPointer( const QWidget *widget );
    static void QtSendSetCursorEvent(wxWindowQt* win, const wxPoint& posClient);

#if wxUSE_ACCEL
    virtual void QtHandleShortcut ( int command );
#endif // wxUSE_ACCEL

#if wxUSE_TOOLTIPS
    // applies tooltip to the widget.
    virtual void QtApplyToolTip(const wxString& text);
#endif // wxUSE_TOOLTIPS

protected:
    virtual void DoGetTextExtent(const wxString& string,
                                 int *x, int *y,
                                 int *descent = nullptr,
                                 int *externalLeading = nullptr,
                                 const wxFont *font = nullptr) const override;

    // coordinates translation
    virtual void DoClientToScreen( int *x, int *y ) const override;
    virtual void DoScreenToClient( int *x, int *y ) const override;

    // capture/release the mouse, used by Capture/ReleaseMouse()
    virtual void DoCaptureMouse() override;
    virtual void DoReleaseMouse() override;

    // freeze/thaw window updates
    virtual void DoFreeze() override;
    virtual void DoThaw() override;

    // retrieve the position/size of the window
    virtual void DoGetPosition(int *x, int *y) const override;

    virtual void DoSetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO) override;
    virtual void DoGetSize(int *width, int *height) const override;

    // same as DoSetSize() for the client size
    virtual void DoSetClientSize(int width, int height) override;
    virtual void DoGetClientSize(int *width, int *height) const override;

    virtual void DoMoveWindow(int x, int y, int width, int height) override;

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip ) override;
#endif // wxUSE_TOOLTIPS

#if wxUSE_MENUS
    virtual bool DoPopupMenu(wxMenu *menu, int x, int y) override;
#endif // wxUSE_MENUS

    // This is called when capture is taken from the window. It will
    // fire off capture lost events.
    void QtReleaseMouseAndNotify();

    // Return the parent to use for children being reparented to us: this is
    // overridden in wxFrame to use its central widget rather than the frame
    // itself.
    virtual QWidget* QtGetParentWidget() const { return GetHandle(); }

    QWidget             *m_qtWindow;
    QAbstractScrollArea *m_qtContainer;  // either nullptr or the same as m_qtWindow pointer
                                         // if m_qtWindow derives from QAbstractScrollArea,
                                         // e.g. QListWidget and QTextEdit.

private:
    void Init();

    // Return the viewport of m_qtContainer, if it's used, or just m_qtWindow.
    //
    // Always returns non-null pointer if the window has been already created.
    QWidget *QtGetClientWidget() const;

    QScrollBar *QtGetScrollBar( int orientation ) const;

    bool QtSetBackgroundStyle();

    std::unique_ptr<QPicture> m_qtPicture;                   // owned by this window
    std::unique_ptr<QPainter> m_qtPainter;                   // always allocated

    bool m_mouseInside;

    bool  m_pendingSize = false; // to properly set the size of the TLW if SetSize()
                                 // is called before the window is shown.

    wxSize  m_pendingClientSize;

#if wxUSE_ACCEL
    wxVector<QShortcut*> m_qtShortcuts; // owned by whatever GetHandle() returns
    std::unique_ptr<wxQtShortcutHandler> m_qtShortcutHandler; // always allocated
    bool m_processingShortcut;
#endif // wxUSE_ACCEL

    wxDECLARE_DYNAMIC_CLASS_NO_COPY( wxWindowQt );
};

#endif // _WX_QT_WINDOW_H_
