MPPlugin
========
Unreal Engine version used 5.4

Setup
-----
*   If you were to use newer version of Unreal Engine and the plugins were not working, copy the plugins from the LYRA template project with the correct Unreal Engine version
*   For the MultiplayerMenu UI to work, you have to set DefaultUIPolicyClass variable path in DefaultGame.ini
*   PlayerControllers have to inherit from ACommonPlayerController for the WidgetPush to work correctly

Menu
![image](https://github.com/user-attachments/assets/359b115d-fa69-40fc-8c33-67f59afef2fd)
Host
![image](https://github.com/user-attachments/assets/c0452f73-7a12-4f02-95b2-ab13f112f868)
Join
![image](https://github.com/user-attachments/assets/3f3c0d41-285e-427c-bed3-94e439a56dfe)

GameData asset values for QuickPlay
![image](https://github.com/user-attachments/assets/ea747f68-7013-45ec-98ff-eabe6fdccc30)
Can easily change quickplay gamedata values to point to different maps or playercounts by changing the values in DA_GameData_QuickPlay asset
