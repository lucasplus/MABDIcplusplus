#include <vtkSmartPointer.h>
#include <vtkAnimationCue.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCommand.h>
#include <vtkAnimationScene.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
 
class CueAnimator
{
public:
  CueAnimator()
    {
      this->SphereSource=0;
      this->Mapper=0;
      this->Actor=0;
    }
 
  ~CueAnimator()
    {
      this->Cleanup();
    }
 
  void StartCue(vtkAnimationCue::AnimationCueInfo *vtkNotUsed(info),
                vtkRenderer *ren)
    {
      cout << "*** IN StartCue " << endl;
      this->SphereSource=vtkSphereSource::New();
      this->SphereSource->SetRadius(0.5);
 
      this->Mapper=vtkPolyDataMapper::New();
      this->Mapper->SetInputConnection(this->SphereSource->GetOutputPort());
 
      this->Actor=vtkActor::New();
      this->Actor->SetMapper(this->Mapper);
 
      ren->AddActor(this->Actor);
      ren->ResetCamera();
      ren->Render();
    }
 
  void Tick(vtkAnimationCue::AnimationCueInfo *info,
            vtkRenderer *ren)
    {
      double newradius=0.1 +
        (static_cast<double>(info->AnimationTime -
                             info->StartTime)/
         static_cast<double>(info->EndTime-info->StartTime)) * 1;
      this->SphereSource->SetRadius(newradius);
      this->SphereSource->Update();
      ren->Render();
    }
 
  void EndCue(vtkAnimationCue::AnimationCueInfo *vtkNotUsed(info),
              vtkRenderer *ren)
    {
      (void)ren;
      // don't remove the actor for the regression image.
//      ren->RemoveActor(this->Actor);
      this->Cleanup();
    }
 
protected:
  vtkSphereSource *SphereSource;
  vtkPolyDataMapper *Mapper;
  vtkActor *Actor;
 
  void Cleanup()
    {
      if(this->SphereSource!=0)
        {
        this->SphereSource->Delete();
        this->SphereSource=0;
        }
 
      if(this->Mapper!=0)
        {
        this->Mapper->Delete();
        this->Mapper=0;
        }
      if(this->Actor!=0)
        {
        this->Actor->Delete();
        this->Actor=0;
        }
    }
};
 
class vtkAnimationCueObserver : public vtkCommand
{
public:
  static vtkAnimationCueObserver *New()
    {
      return new vtkAnimationCueObserver;
    }
 
  virtual void Execute(vtkObject *vtkNotUsed(caller),
                       unsigned long event,
                       void *calldata)
    {
      if(this->Animator!=0 && this->Renderer!=0)
        {
        vtkAnimationCue::AnimationCueInfo *info=
          static_cast<vtkAnimationCue::AnimationCueInfo *>(calldata);
        switch(event)
          {
          case vtkCommand::StartAnimationCueEvent:
            this->Animator->StartCue(info,this->Renderer);
            break;
          case vtkCommand::EndAnimationCueEvent:
            this->Animator->EndCue(info,this->Renderer);
            break;
          case vtkCommand::AnimationCueTickEvent:
            this->Animator->Tick(info,this->Renderer);
            break;
          }
        }
      if(this->RenWin!=0)
        {
        this->RenWin->Render();
        }
    }
 
  vtkRenderer *Renderer;
  vtkRenderWindow *RenWin;
  CueAnimator *Animator;
protected:
  vtkAnimationCueObserver()
    {
      this->Renderer=0;
      this->Animator=0;
      this->RenWin=0;
    }
};
 
int main(int, char *[])
{
  // Create the graphics structure. The renderer renders into the
  // render window.
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->SetMultiSamples(0);
  iren->SetRenderWindow(renWin);
  renWin->AddRenderer(ren1);
  renWin->Render();
 
  // Create an Animation Scene
  vtkSmartPointer<vtkAnimationScene> scene =
    vtkSmartPointer<vtkAnimationScene>::New();
 
  scene->SetModeToRealTime();
  //scene->SetModeToSequence();
 
  scene->SetLoop(0);
  scene->SetFrameRate(5);
  scene->SetStartTime(3);
  scene->SetEndTime(20);
 
  // Create an Animation Cue.
  vtkSmartPointer<vtkAnimationCue> cue1 =
    vtkSmartPointer<vtkAnimationCue>::New();
  cue1->SetStartTime(5);
  cue1->SetEndTime(23);
  scene->AddCue(cue1);
 
  // Create cue animator;
  CueAnimator animator;
 
  // Create Cue observer.
  vtkSmartPointer<vtkAnimationCueObserver> observer =
    vtkSmartPointer<vtkAnimationCueObserver>::New();
  observer->Renderer=ren1;
  observer->Animator=&animator;
  observer->RenWin=renWin;
 
  cue1->AddObserver(vtkCommand::StartAnimationCueEvent,observer);
  cue1->AddObserver(vtkCommand::EndAnimationCueEvent,observer);
  cue1->AddObserver(vtkCommand::AnimationCueTickEvent,observer);
 
  scene->Play();
  scene->Stop();
 
  iren->Start();
 
  return EXIT_SUCCESS;
}