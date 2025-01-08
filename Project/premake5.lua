-- premake5.lua
workspace "MAGI"
   configurations { "Debug", "Release" }
   platforms { "x64" } -- プラットフォームの設定

local externals_dir = "Externals"  -- 外部ライブラリディレクトリの基準パス

project "MAGI" -- プロジェクト名
   kind "WindowedApp" -- デスクトップアプリケーションに設定
   language "C++"
   cppdialect "C++20" -- 言語の設定

   targetdir "../generated/bin/%{cfg.buildcfg}/%{cfg.platform}"  -- 実行ファイルの出力先
   objdir "../generated/obj/%{cfg.buildcfg}/%{cfg.platform}"     -- 中間ファイルの出力先
   
   includedirs { "Engine", "App","Externals/assimp/include" } -- 追加のインクルードパス

   files { "**.h", "**.cpp" }

   warnings "High" -- 警告レベル3を設定

   buildoptions { "/utf-8" } -- UTF-8でビルドする設定

   flags { "MultiProcessorCompile" } -- 複数プロセッサでのコンパイルを有効化

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      fatalwarnings { "All" }  -- 警告をエラーとして扱う
      staticruntime "On"  -- 静的ランタイム（/MTd）

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      staticruntime "On"  -- 静的ランタイム（/MT）