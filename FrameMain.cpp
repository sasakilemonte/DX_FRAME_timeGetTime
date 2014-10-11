//フレームアニメーションのベースプログラムです。
//時間の取得にtimeGetTime()を使用しているため、
//このキーフレームアニメーションのひな形を使うには
//プロジェクトのプロパティの項目、
//構成プロパティのリンカーの追加の依存ファイルにwinmm.libを追加のこと
//(C)2013 lemonte.net/佐々木順昭

//[DXライブラリ置き場]http://homepage2.nifty.com/natupaji/DxLib/
//[VisualStudio2012でのDXライブラリの使い方]http://homepage2.nifty.com/natupaji/DxLib/dxuse_vc2012.html
#include "DxLib.h"

//------------------------------------------------
//ゲームの動作用グローバル変数をここに記述します
//------------------------------------------------







//---------------------------------------------------------
//ゲームの動作用　関数プロトタイプ宣言をここに記述します
//---------------------------------------------------------







//----------------------------------------------------------
//フレームレート設定(16.66666は1000ミリ秒=60フレームの意味)
#define Framerate 16.6666
//----------------------------------------------

//--------------------------------------
//デバッグ用コンソール出力
//#define console_debug
//--------------------------------------

//-----------------------------------------------
//サンプルのマウス座標と左ボタンの検出表示用です
//要らない場合は削除しても良いです。

//マウス左ボタンが押されているか？
bool mouseDown=false;
//マウスX座標
int mouseX   =0;
//マウスY座標
int mouseY   =0;
//マウス左ボタンが押されているときのカウントアップ用です。
int mouseCnt=0;
//-----------------------------------------------

//--------------------------------------
//サンプルのフレームカウントです
//要らない場合は削除しても良いです。
int frame_count=0;
//--------------------------------------

//プロトタイプ宣言
void task_init();
void task_content_load();
void task_update();
void task_draw();

//ウィンドウズメイン関数です。
//Windowsのプログラムにおいて、一番最初に呼ばれる関数です。
//[WinMain]http://msdn.microsoft.com/ja-jp/library/cc364870.aspx
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 LPSTR lpCmdLine, int nCmdShow )
{
	//初期化処理の呼び出し
	task_init();

	// タイトルを test に変更(必須)
	//[SetMainWindowText]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_other.html#R15N13
	SetMainWindowText( "フレームアニメーション(timeGetTime使用)" ) ;

	//ウィンドウモードで起動(必須)(TRUEならウィンドウモードfalseならフルスクリーンモードで動作します)
	//[ChangeWindowMode]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_other.html#R11N1
    ChangeWindowMode( TRUE );

	//画面モードの設定(必須)
	//画面の広さ800ｘ600ピクセルで、
	//16ビットはDXライブラリの標準色ビット数で、65536色を使って画像を表現します。
	//[SetGraphMode]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N1
	SetGraphMode( 800 , 600 , 16 ) ;

	// ＤＸライブラリ初期化処理(必須)
	//[DxLib_Init()]http://homepage2.nifty.com/natupaji/DxLib/dxfunc.html#R1N1
	if( DxLib_Init() == -1 ) return -1;

	//グラフィック、サウンドなどのリソース読み込みです。
	task_content_load();

	//コンソール出力のマクロ定義です。
	//ソースコードの最初の方で　#define console_debug　と記述すれば
	//#ifdef~#endifの間はコンパイル時に挿入されて実行ファイルが出来上がります。
	#ifdef console_debug
		AllocConsole();
        freopen("CONOUT$", "w", stdout); 
        freopen("CONIN$", "r", stdin);
	#endif

	// マウスを表示状態にする(TRUEなら表示、falseなら表示しない)
	//http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N5
	SetMouseDispFlag( TRUE ) ;

	// グラフィックの描画先を裏画面にセット(必須)(初回に必要)
	//[SetDrawScreen]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N6
	SetDrawScreen( DX_SCREEN_BACK ) ;

	//ゲームの動作更新の最終時間です。
	//[timeGetTime()]http://msdn.microsoft.com/ja-jp/library/cc428795.aspx
	double lasttime = timeGetTime() ;
	//動作更新の最終時間を現在の時間に設定する。

	//フレームアニメーションの早送り回数の変数です。
	int loop_count=0;

	//-----------------------------------------
	//フレームアニメーションの無限ループです。
	//-----------------------------------------
	for(;;)
	{
		//経過時間により、アップデートを実行する。
		//フレーム落ち発生の場合はアップデートを繰り返し呼び出し、
		//実行したことにする。

		//現在時間の取得用変数です。
		//[timeGetTime()]http://msdn.microsoft.com/ja-jp/library/cc428795.aspx
		double curtime=timeGetTime();
		//現在時間に設定

		//現在庫画面更新時間から前回の画面更新時間を引いた値を
		//ループタイムとします。
		double looptime=curtime-lasttime;

		//ループタイムがFramerateを超えた場合、更新処理を行います。
		//このとき、もし16.6666フレームで更新のとき
		//16のときループカウントは0に
		//18のときループカウントは1に
		//35のときはループカウント2になるよう
		//動作するプログラムにしたい。
		if (looptime>Framerate)
		{
			loop_count=(int)(looptime/Framerate);	//ループ回数を求める
			double amaritime=looptime-(loop_count*Framerate);	//ループ回数の余り時間を求める
			lasttime=curtime-amaritime;

			//ゲームの更新処理です。フレーム落ちが起こっている場合、
			//複数回実行(早送り)してアニメーションフレームを進ませます。
			int l=0;	//更新回数が0の場合は動作更新処理を行いません。
			while(l<loop_count)
			{
				task_update();	//動作更新処理呼び出し
				l++;
			}
		}
		//アップデート処理終了--------------------------------------------

		//描画更新処理の呼び出し(ゲームの更新処理を行ったときに描画更新を行う)
		if (loop_count>0)
		{
			task_draw();
		}

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// -1 が返ってきたらループを抜ける
		if( ProcessMessage() < 0 ) break ;

		// もしＥＳＣキーが押されていたらループから抜ける
		//[CheckHitKey]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N2
		if( CheckHitKey( KEY_INPUT_ESCAPE ) ) break ;

	}	//(for(;;)　フレーム用無限ループの中括弧です(削除禁止)
	//--------------------------------------------------------------------------------------





	//コンソール出力解放のマクロ定義です。
	//ソースコードの最初の方で　#define console_debug　と記述すれば
	//#ifdef~#endifの間はコンパイル時に挿入されて実行ファイルが出来上がります。
	#ifdef console_debug
        //コンソール解放
        FreeConsole();
	#endif
	//----------------------------------------------------------------------------------------

	// ＤＸライブラリ使用の終了処理(必須)
	//[DxLib_End]http://homepage2.nifty.com/natupaji/DxLib/dxfunc.html#R1N2
	DxLib_End() ;				
	//----------------------------------------------------------------------------------------

	// ソフトの終了
	return 0 ;
}




//最初の初期化処理の部分
//----------------------------------------------
//一番最初に実行する初期化の処理です。
//変数の初期化やデータのロードなどを行います。
//----------------------------------------------
void task_init()
{






}

//最初のグラフィック、サウンドの読み込み部分
//------------------------------------------------------
//DXライブラリの初期化ができていないと、グラフィック
//サウンドなどのデータを読み込むことができません。
//------------------------------------------------------
void task_content_load()
{





}

//動作更新の部分
//-----------------------------------------
//自由にゲームの動作処理を書くところです。
//設定した値での動作更新を行います。
//-----------------------------------------
void task_update()
{
	//---------------------------------------
	//ゲームの動作処理をここに書きます
	//---------------------------------------



















	// マウスの位置を取得(使わない場合は不要)
	//[GetMousePoint]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N6
	GetMousePoint( &mouseX , &mouseY ) ;
	//----------------------------------------------------------------------------------------

	//マウスのクリック処理(使わない場合は不要)
	//[GetMouseInput]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N8
	if( ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 )
	{
		mouseCnt++;
		if (mouseCnt>24) mouseCnt=24;
	}else mouseCnt=0;
	//----------------------------------------------------------------------------------------

	//フレームカウントアップ(使わない場合は不要)
	frame_count++;
	//----------------------------------------------------------------------------------------

}

//描画更新の部分
//-----------------------------------------------
//描画の処理を書くところです。
//フレーム落ちなどの要因で不定期で実行されるので
//ここでは動作の記述を行ってはいけません
//-----------------------------------------------
void task_draw()
{
	// 画面を初期化(真っ黒にする)(使わない場合は不要)
	//[ClearDrawScreen()]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N4
	ClearDrawScreen() ;
	//----------------------------------------------------------------------------------------

	//---------------------------------------
	//ゲームの描画処理をここに書きます
	//---------------------------------------










	//文字色を白に(使わない場合は不要)
	//[GetColor]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N5
	int Cr = GetColor( 255 , 255 , 255 ) ;
	//----------------------------------------------------------------------------------------

	//グラフィックの表示(使わない場合は不要)
	//[DrawFormatString]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph2.html#R17N20
	DrawFormatString(400, 10, Cr, "MOUSE  X=%d Y=%d Cnt=%d \n", mouseX,mouseY,mouseCnt) ;
	//----------------------------------------------------------------------------------------

	//フレームカウント表示(使わない場合は不要)
	DrawFormatString(100, 10, Cr, "frame count=%d",frame_count/60);
	//----------------------------------------------------------------------------------------

	// 裏画面の内容を表画面にコピーする(必須)
	//ダブルバッファのフリップ(更新)処理のため、この後に描画処理を書いてはいけません
	//[ScreenFlip()]http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_graph3.html#R4N7
	ScreenFlip() ;
	//----------------------------------------------------------------------------------------
}



//---------------------------------------------------------
//ゲームの動作用　関数をここから記述します
//---------------------------------------------------------

