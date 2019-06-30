#include "imagebrowser.h"

ImageBrowser::ImageBrowser(wxFrame* parent, const wxString& filename)
  : wxPanel(parent, wxID_ANY, wxPoint(-1, -1), wxSize(1024, 768))
{
  image = new wxImage(filename);
  Bind(wxEVT_PAINT, &ImageBrowser::OnPaint, this);
}

ImageBrowser::~ImageBrowser()
{
  delete image;
}

void ImageBrowser::OnPaint(const wxPaintEvent& evt)
{
  int x = 480 - image->GetWidth() / 2;    // 480, 320是试出来的，一定有别的方法
  int y = 320 - image->GetHeight() / 2;
  
  wxBitmap pic = wxBitmap(*image);
  wxPaintDC dc(this);
  dc.DrawBitmap(pic, x, y, false);
}

MyFrame::MyFrame(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1), wxSize(1024, 768), wxDEFAULT_FRAME_STYLE & (~wxRESIZE_BORDER))
{
  imagePane = NULL;
  n_in_imageFiles = 0;
  imageDir = wxT("");
  imageFiles.Alloc(5);
  wxMenu* file = new wxMenu;
  wxMenuBar* menuBar = new wxMenuBar;
  vbox = new wxBoxSizer(wxVERTICAL);
  SetSizer(vbox);
  
  file->Append(wxID_OPEN, wxT("打开"));
  file->Append(wxID_EXIT, wxT("退出"));
  menuBar->Append(file, wxT("文件"));
  SetMenuBar(menuBar);

  Bind(wxEVT_MENU, &MyFrame::OnOpenFile, this, wxID_OPEN);
  Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_EXIT);
  Bind(wxEVT_MOUSEWHEEL, &MyFrame::OnMouseRoll, this);      // 处理鼠标事件
}

void MyFrame::OnMouseRoll(const wxMouseEvent& evt)
{
  static int i = 0;
  if (imagePane != NULL)
    delete imagePane;
  if (n_in_imageFiles > 0) {
    i = (++i) % n_in_imageFiles;
    imagePane = new ImageBrowser(this, imageFiles[i]);
    vbox->Add(imagePane, 1, wxEXPAND);
    SetSizer(vbox);
  }
}

void MyFrame::OnOpenFile(const wxCommandEvent& evt)
{
  wxFileDialog openFileDialog(this, wxT("Open Image File"), "", "", wxT("JPEG files (*.jpg)|*.jpg|BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|PNG files (*.png)|*.png"));

  if (imagePane != NULL)
    delete imagePane;
  if (openFileDialog.ShowModal() == wxID_OK) {
    filename = openFileDialog.GetPath();    // 需要判断文件是不是图像文件
    imagePane = new ImageBrowser(this, filename);
    vbox->Add(imagePane, 1, wxEXPAND);
    SetSizer(vbox);
    
    wxFileName fullName(filename);
    if (imageDir != fullName.GetPath()) {         // 又打开了一个新的可能包含图像文件的目录，更改目录下的文件
      imageDir = fullName.GetPath();
      imageFiles.Clear();                         // 清空原来的内容
      wxDir::GetAllFiles(imageDir, &imageFiles, wxT("*.gif"));
      wxDir::GetAllFiles(imageDir, &imageFiles, wxT("*.jpg"));
      wxDir::GetAllFiles(imageDir, &imageFiles, wxT("*.png"));      
      n_in_imageFiles = imageFiles.GetCount();
    }
  }
}

void MyFrame::OnClose(const wxCommandEvent& evt)
{
  Close();
}

class MyApp : public wxApp
{
private:
  MyFrame* frame;
public:
  virtual bool OnInit()
  {
    wxInitAllImageHandlers();

    frame = new MyFrame(wxT("Image Browser"));
    frame->Show();
    return true;
  }
};
  IMPLEMENT_APP(MyApp)
