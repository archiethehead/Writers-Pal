using LibGit2Sharp;
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Avalonia.Platform;

namespace Writers_Pal.Models {

    public static class VersionControl {

        public static Repository LoadLocalRepo(string filepath) {

            return new Repository(filepath);

        }

        public static Repository CreateNewRepo(string filepath) {

            if (!Repository.IsValid(filepath)) {

                Repository.Init(filepath);
                
            }

            return LoadLocalRepo(filepath);
        
        }

        public static void CommitRepoChanges(Signature author, Repository currentRepository, string commitMessage) {

            if (!currentRepository.RetrieveStatus().IsDirty) return;

            Commands.Stage(currentRepository, "*");
            currentRepository.Commit(commitMessage, author, author);

        }

        public static void MergeRepoBranch(Repository currentRepository, Branch branch, Signature author) {

            Commands.Checkout(currentRepository, currentRepository.Head.FriendlyName);
            MergeResult result = currentRepository.Merge(branch, author);

            // Will implement error notifications later.
            if (result.Status == MergeStatus.Conflicts) return;

        }

        public static void CreateRepoBranch(Repository currentRepository, string branchName) {

            Branch newBranch = currentRepository.CreateBranch(branchName);
            switchBranch(currentRepository, newBranch);

        }

        public static void switchBranch(Repository currentRepository, Branch branch) {

            Commands.Checkout(currentRepository, branch);

        }

        public static void DeleteRepo(string filepath) {

            DirectoryInfo DI = new DirectoryInfo(filepath);

            foreach (FileInfo file in DI.GetFiles("*", SearchOption.AllDirectories)) {

                file.IsReadOnly = false;

            }

            if (Directory.Exists(filepath)) {

                Directory.Delete(filepath, recursive: true);

            }

        }


    }

}
