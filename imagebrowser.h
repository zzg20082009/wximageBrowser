#ifndef __IMAGE_BROWSER__
#define __IMAGE_BROWSER__
#include <wx/wxprec.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/filename.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#endif

class ImageBrowser : public wxPanel
{
 public:
  ImageBrowser(wxFrame* parent, const wxString& filename);
  ~ImageBrowser();
 private:
  wxImage* image;
  void OnPaint(const wxPaintEvent& evt);
};

class MyFrame : public wxFrame
{
 public:
  MyFrame(const wxString& title);
 private:
  wxSizer* vbox;
  ImageBrowser* imagePane;
  wxString filename;                            // 具体的图像文件
  wxString imageDir;                            // 存放图片的路径
  wxArrayString imageFiles;                     // 存放图片目录下的文件
  size_t n_in_imageFiles;
  void OnOpenFile(const wxCommandEvent& evt);
  void OnClose(const wxCommandEvent& evt);
  void OnMouseRoll(const wxMouseEvent& evt);    // 处理鼠标事件
};

  
