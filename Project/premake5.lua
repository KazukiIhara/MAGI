workspace "MAGI"
   configurations { "Debug", "Release" }
   platforms { "x64" }
   startproject "MAGI" -- デフォルトのスタートプロジェクトを指定

   -- 出力ディレクトリの設定
   targetdir "../generated/bin/%{cfg.buildcfg}/%{cfg.platform}"  -- 実行ファイルの出力先
   objdir "../generated/obj/%{cfg.buildcfg}/%{cfg.platform}"     -- 中間ファイルの出力先


-- MAGIの設定

project "MAGI" -- プロジェクト名
   kind "WindowedApp" -- デスクトップアプリケーションに設定
   language "C++"
   cppdialect "C++20" -- 言語の設定

   -- プロジェクトに含むファイル
   files { 
      "*.cpp",
      "*.h",
      "Engine/**.cpp",
      "Engine/**.h",
   }
   
   -- 追加のインクルードパス
   includedirs { 
   "Engine",
   "App",
   "Externals/assimp/include",
    } 

   warnings "High" -- 警告レベル3を設定

   buildoptions { "/utf-8" } -- UTF-8でビルドする設定

   flags { "MultiProcessorCompile" } -- 複数プロセッサでのコンパイルを有効化

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      fatalwarnings { "All" }  -- 警告をエラーとして扱う
      staticruntime "On"  -- 静的ランタイム（/MTd）
      linkoptions { "/IGNORE:4049", "/IGNORE:4099" } -- 指定したリンカーの警告を無視
      libdirs { "Externals/assimp/lib/Debug" } -- デバッグ用追加のライブラリディレクトリ
      links { "assimp-vc143-mtd" } -- デバッグ用ライブラリ

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      staticruntime "On"  -- 静的ランタイム（/MT）
      linkoptions { "/IGNORE:4049", "/IGNORE:4099" } -- 指定したリンカーの警告を無視
      libdirs { "Externals/assimp/lib/Release" } -- リリース用追加のライブラリディレクトリ
      links { "assimp-vc143-mt" } -- リリース用ライブラリ
