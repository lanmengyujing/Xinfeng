//
// ********************************************************************
// ���ԣ���������ͼ����GPSԴ
// 2012-9-24
// ********************************************************************
//       
// --------------------------------------------------------------------
//             espec.cc  ������
// --------------------------------------------------------------------

//G4RunManager��ʼ��ģ����Ϣ�����������ӳ���
#include "G4RunManager.hh"
//G4UImanagerΪUI�������������˻�����
#include "G4UImanager.hh"

//#include "G4ScoringManager.hh"

//especDetectorConstruction���νṹ�趨
#include "especDetectorConstruction.hh"
//especPhysicsList�����趨
//#include "especPhysicsList.hh"
//especPrimaryGeneratorActionԴ����
#include "especPrimaryGeneratorAction.hh"

//��ѡ�ࣺRunAction ���д����࣬EventAction �¼������࣬SteppingAction �����ݴ����࣬TrackingAction ����������
//�����������Ȥ����Ϣ
#include "especRunAction.hh"
#include "especEventAction.hh"

//#include "especHistoManager.hh"
//#include "especUserScoreWriter.hh"

#include "QGSP_BIC_HP.hh"
//#include "G4PhysListFactory.hh"

//�Ƿ�궨����G4VIS_USE������ǾͰ���G4VisExecutiveͷ�ļ�
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif



int main(int argc,char** argv)        //argc���������ַ���������argv[]���ַ�������
{
	//����һ�����й�������ʵ��
	G4RunManager * runManager = new G4RunManager;

	 //G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
	 //scManager->SetVerboseLevel(1);
	 //scManager->SetScoreWriter(new especUserScoreWriter());


	//��ʼ���ࣨ3��ǿ�����е�2����
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
	
	//�û������ࣨԴ������ǿ����Ҳ���û������ࣩ
	especPrimaryGeneratorAction* gen_action = new especPrimaryGeneratorAction;
	runManager->SetUserAction(gen_action);
	//
	//�û������ࣨ��ѡ�ࣩ
	especRunAction* run_action = new especRunAction();
	runManager->SetUserAction(run_action);

	especEventAction* event_action = new especEventAction();
	runManager->SetUserAction(event_action);

	//runManager->SetUserAction(new RunAction);
	//runManager->SetUserAction(new EventAction);
	//runManager->SetUserAction(new SteppingAction);

	//G4�ں˳�ʼ��������̽����������������̣�������沢����run
	runManager->Initialize();

	//----------intercoms��--------------
#ifdef G4VIS_USE         //���������G4VIS_USE���������ӻ�������visManager
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	
	//����UI������, �û��ӿڹ�����
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	
	//G4UIExecutive��G4UIsession����������
	if (argc==1)            //����ʽUI�նˣ�����������ַ���ֻ��1����ִ������{}
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
	else                   //������ģʽ
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

    // ����3��˳��ִ�е��¼��ĵ�һ��run��
    // ����run֮������޸�̽�����ṹ���������
	// �öο��Է�����ļ���ִ��
    //  int numberOfEvent = 3;
    //  runManager->BeamOn(numberOfEvent);

#ifdef G4VIS_USE
	delete visManager;
#endif

	delete runManager;

	return 0;
}
