
----------------------------------------------------------------
-- ① ユーザー設定
----------------------------------------------------------------
local StaticRuntime = "On"                                
local DXC_DIR       = "Externals/dxc_1_8/bin/x64"         -- ← プロジェクト内 DXC 配置先
local DXC_EXE       = path.join(DXC_DIR, "dxc.exe")       -- dxc.exe へのフルパス

----------------------------------------------------------------
-- ② 共通パス
----------------------------------------------------------------
local OUT_DIR = "../generated/outputs/%{cfg.buildcfg}/%{cfg.platform}"
local OBJ_DIR = "../generated/obj/%{prj.name}/%{cfg.buildcfg}"
local THIRD   = "Externals"

----------------------------------------------------------------
-- ③ ワークスペース
----------------------------------------------------------------
workspace "MAGI"
    configurations { "Debug", "Release" }
    platforms      { "x64" }
    startproject   "MAGI"
    location       "./"

    -- 共通ビルドオプション
    warnings      "High"
    buildoptions  { "/utf-8", "/MP" }
    linkoptions   { "/IGNORE:4049", "/IGNORE:4099" }
    flags         { "MultiProcessorCompile" }

----------------------------------------------------------------
-- ④ 外部ライブラリ (DirectXTex / imgui)
----------------------------------------------------------------
local externals = {
    { name="DirectXTex", path=THIRD.."/DirectXTex", uuid="12345678-ABCD-4321-DCBA-1234567890AB" },
    { name="imgui",      path=THIRD.."/imgui",      uuid="22345678-ABCD-4321-DCBA-1234567890AB" },
}

for _, lib in ipairs(externals) do
    externalproject (lib.name)
        location (lib.path)
        filename (lib.name)
        uuid     (lib.uuid)
        kind     "StaticLib"
        language "C++"
end

----------------------------------------------------------------
-- ⑤ MAGI 本体
----------------------------------------------------------------
project "MAGI"
    kind       "WindowedApp"
    language   "C++"
    cppdialect "C++20"

    targetdir (OUT_DIR)
    objdir    (OBJ_DIR)

    files {
        "*.cpp", "*.h",
        "Engine/**.cpp", "Engine/**.h", "Engine/**.ipp",
        "App/**.cpp",    "App/**.h",
        "Externals/d3dx12/**.h",
    }

    includedirs {
        "Engine", "Engine/Includes", "App",
        THIRD,
        THIRD.."/assimp/include"
    }

    dependson { "DirectXTex", "imgui" }
    links     { "DirectXTex", "imgui" }

    ------------------------------------------------------------
    -- ⑤‑A HLSL : カスタムビルドで DXC 1.8 を直接呼び出す
    ------------------------------------------------------------
    -- プロファイル別にフィルターを張る (拡張子末尾規約: *.VS.hlsl 等)
    local shaderProfiles = {
        { pattern="**.*VS.hlsl", profile="vs_6_5" },
        { pattern="**.*PS.hlsl", profile="ps_6_5" },
        { pattern="**.*CS.hlsl", profile="cs_6_5" },
        { pattern="**.*MS.hlsl", profile="ms_6_5" },
        { pattern="**.*AS.hlsl", profile="as_6_5" },
    }

    for _, sp in ipairs(shaderProfiles) do
        filter { "files:" .. sp.pattern }
            shadermodel   "6.5"           -- RTX 20 系に最適な Shader Model
            shaderentry   "main"
            buildaction   "None"          -- VS の FxCompile を無効化
            buildmessage  '[DXC] %{file.relpath}'

            buildcommands {
                string.format(
                    '"%s" -E main -T %s -Zi -Fo "%%{cfg.objdir}/%%{file.basename}.cso" "%%{file.relpath}"',
                    DXC_EXE, sp.profile
                )
            }

            buildoutputs { "%{cfg.objdir}/%{file.basename}.cso" }
        filter {}
    end

    -- 生成した .cso を自動リンク対象へ
    compilebuildoutputs()

    ------------------------------------------------------------
    -- ⑤‑B Debug / Release 切替
    ------------------------------------------------------------
    filter "configurations:Debug"
        defines       { "DEBUG" }
        symbols       "On"
        staticruntime (StaticRuntime)
        libdirs       { THIRD.."/assimp/lib/Debug" }
        links         { "assimp-vc143-mtd" }

    filter "configurations:Release"
        defines       { "NDEBUG" }
        optimize      "On"
        staticruntime (StaticRuntime)
        libdirs       { THIRD.."/assimp/lib/Release" }
        links         { "assimp-vc143-mt" }
    filter {}

    ------------------------------------------------------------
    -- ⑤‑C ビルド後 : DXIL ランタイム DLL をコピー
    ------------------------------------------------------------
    postbuildcommands {
        ('copy /Y "%s\\dxil.dll"       "%%(TargetDir)"'):format(DXC_DIR),
        ('copy /Y "%s\\dxcompiler.dll" "%%(TargetDir)"'):format(DXC_DIR),
    }
