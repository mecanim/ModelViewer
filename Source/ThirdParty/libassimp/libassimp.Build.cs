using UnrealBuildTool;
using System.IO;

public class libassimp : ModuleRules
{
    public libassimp(ReadOnlyTargetRules Target) : base(Target)
    {
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // We neeed to depend on Core in order to implement our game module in the next step
        PublicDependencyModuleNames.AddRange(new string[]{"Core"});

        PublicIncludePaths.Add(ModuleDirectory + "\\include");
        PublicAdditionalLibraries.Add(ModuleDirectory + "\\lib\\assimp-vc141-mt.lib");

    }
}