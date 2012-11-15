//
// ********************************************************************
// 测试，画出能谱图，用GPS源
// 2012-9-24
// ********************************************************************
//       
// --------------------------------------------------------------------
//             espec.cc  主程序
// --------------------------------------------------------------------

//G4RunManager初始化模拟信息，用于连接子程序
#include "G4RunManager.hh"
//G4UImanager为UI管理器，用于人机交互
#include "G4UImanager.hh"

//#include "G4ScoringManager.hh"

//especDetectorConstruction几何结构设定
#include "especDetectorConstruction.hh"
//especPhysicsList物理设定
//#include "especPhysicsList.hh"
//especPrimaryGeneratorAction源描述
#include "especPrimaryGeneratorAction.hh"

//可选类：RunAction 运行处理类，EventAction 事件处理类，SteppingAction 步数据处理类，TrackingAction 径迹处理类
//用来输出感兴趣的信息
#include "especRunAction.hh"
#include "especEventAction.hh"

//#include "especHistoManager.hh"
//#include "especUserScoreWriter.hh"

#include "QGSP_BIC_HP.hh"
//#include "G4PhysListFactory.hh"

//是否宏定义了G4VIS_USE，如果是就包含G4VisExecutive头文件
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif



int main(int argc,char** argv)        //argc：命令行字符串个数，argv[]：字符串数组
{
	//创建一个运行管理器的实例
	G4RunManager * runManager = new G4RunManager;

	 //G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
	 //scManager->SetVerboseLevel(1);
	 //scManager->SetScoreWriter(new especUserScoreWriter());


	//初始化类（3个强制类中的2个）
	especDetectorConstruction* detector = new especDetectorConstruction;
	runManager->SetUserInitialization(detector);
	//
	//especPhysicsList* physicsList= new especPhysicsList();
	//runManager->SetUserInitialization(physicsList);

	G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
	runManager->SetUserInitialization(physicsList);

	//G4PhysListFactory *physListFactory = new G4PhysListFactory();
	//G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BIC_HP");
	//G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
	//runManager->SetUserInitialization(physicsList);
	
	//用户操作类（源描述是强制类也是用户操作类）
	especPrimaryGeneratorAction* gen_action = new especPrimaryGeneratorAction;
	runManager->SetUserAction(gen_action);
	//
	//用户操作类（可选类）
	especRunAction* run_action = new especRunAction();
	runManager->SetUserAction(run_action);

	especEventAction* event_action = new especEventAction();
	runManager->SetUserAction(event_action);

	//runManager->SetUserAction(new RunAction);
	//runManager->SetUserAction(new EventAction);
	//runManager->SetUserAction(new SteppingAction);

	//G4内核初始化，建立探测器，创建物理过程，计算截面并建立run
	runManager->Initialize();

	//----------intercoms类--------------
#ifdef G4VIS_USE         //如果定义了G4VIS_USE，创建可视化管理器visManager
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	
	//创建UI管理器, 用户接口管理类
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	
	//G4UIExecutive和G4UIsession有神马区别
	if (argc==1)            //交互式UI终端：如果命令行字符串只有1个，执行下面{}
    {
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute gps.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
    }
	else                   //批处理模式
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

    // 运行3个顺序执行的事件的第一个run，
    // 两次run之间可以修改探测器结构和物理过程
	// 该段可以放入宏文件中执行
    //  int numberOfEvent = 3;
    //  runManager->BeamOn(numberOfEvent);

#ifdef G4VIS_USE
	delete visManager;
#endif

	delete runManager;

	return 0;
}
