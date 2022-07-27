# Instructions for git
_____
_____

## General remarks

Read carefully what git writes to you. In most cases its advice is correct, as you will see in the instructions below.

Make sure with `git status` that you are on the proper branch. Change branch with `git checkout`, when needed.

_____
## `git status` and commiting

Commit is like a saving checkpoint -- use it each time you want to preserve something for the future. It is better to have many small commits rather than a single big change because:
- you can be more specific in each commit message, so you can more easily find a specific change in history
- it is easier to analyze, step-by-step, what are your changes
- it makes rebasing and other history manipulations easier
- in case of bugs, it is easier to find when the bug appeared and which particular change triggered it

`git status` output:
- *changes to be committed*: these modifications will be included in the next commit made with `git commit -m "Commit message"`; they are not included in `git diff` output
- *changes not staged for commit*: you can preview them with `git diff` and, once you are happy with all changes made to a given file `foo.cxx`, you can add (stage) the file for the next commit with `git add foo.cxx`
- *untracked files*: these files are not included in the git repository; use `git add` to start tracking a given file

**NOTE:** Often, you might want to commit all existing changes instead of adding files one by one. `git commit -am "Commit message"` automatically adds all unstaged files and commits them. This **doesn't** include *untracked* files since they are excluded from the repository.

_____
## `git fetch`, `git push`, `git pull` -- working with the remote repository

All these commands require Internet connection.

`git fetch` downloads history of branches on remote repository. By default, it's `origin`, you can specify another repository (e.g., `upstream`).

`git push` pushes the newest commits from the current branch to the corresponding (*tracked*) branch on the remote.<br>
`git push -f` forces the remote to accept the local version of the history, in case the histories differ, for example, after a rebase.<br>

`git pull` adds the new commits from the remote branch on the top of the local branch. It doesn't work well if you have any local commits which are not included in the remote. Therefore, always prefer `git rebase`. Rebase will work also when you have no local commits.

### Current branch has no upstream branch

If the branch is new and there is no corresponding branch on remote, `git push` fails with:
```
fatal: The current branch my-branch-name has no upstream branch.
To push the current branch and set the remote as upstream, use

    git push --set-upstream origin my-branch-name
```
Just copy-paste-run the command git advices.

If there is already a branch on remote but somewhat the tracking got interrupted, and you want to track that remote branch, do:
```
git fetch origin
git branch --set-upstream-to=origin/my-branch-name
```

_____
## `git rebase` -- update your current branch or PR to newest upstream

If you want to update the master branch of your forked repository (the `origin` repository in `git remote -vvv` and `git branch -vvv` output):
```
git fetch upstream
git rebase upstream/master
```

If you want to update any other branch, you can use the same steps.

You might also want to update your branch on top of your local master (for example: your master is regularly updated, you have weak Internet connection and `git fetch` fails to download newest changes from upstream). Then simply call `git rebase master`.

### Uncommited changes

`git rebase` can fail with a message about changes not commited and a warning that the rebase would modify/delete the changed files. Git automatically aborts the rebase in such cases and allows you to take care yourself about these modifications.

Use `git status` to see what files are causing the problem.

As usual: use `git diff` to preview the uncommitted changes. `git add`/`git commit` for adding the modifications and commiting them.

When git shows no *changes to be committed* and no *changes not staged for commit*, you can run `git rebase` again.

### Merge conflicts

Git fails with an instruction like:
```
CONFLICT (content): Merge conflict in todo.txt
error: Failed to merge in the changes.
Patch failed at 0003 Some commit message
hint: Use 'git am --show-current-patch' to see the failed patch
Resolve all conflicts manually, mark them as resolved with
"git add/rm <conflicted_files>", then run "git rebase --continue".
You can instead skip this commit: run "git rebase --skip".
To abort and get back to the state before "git rebase", run "git rebase --abort".
```

Use `git status` to see what files are causing the problem. The resolved files appear under *changes to be committed* while the conflicts are under *changes not staged for commit*.

As git says, you need to resolve all conflicts manually: open each of the conflict files and search for lines like:
```
<<<<<<< HEAD
summy
=======
dummy
>>>>>>> Some commit message
```

The code between `HEAD` and `===` was modified in the branch you are rebasing on (e.g., upstream/master). The code between `===` and `>>>> Some commit message` was modified in the current branch in the commit with this message. You need to decide which version is correct.<br>
Sometimes, it might happen you will want to preserve a part of the HEAD content and a part of your changes -- this is also allowed! The only thing git cares about is to remove these special lines: `HEAD`, `====` and `>>>> Some commit message`. You need to care yourself that the remaining code makes sense.

Then, we follow the next git advice: after you are happy with a specific file, use `git add foo.cxx`. In the context of rebase, this signals to git that the file is no longer conflicting.<br>
If there is some spurious file that is included in rebase but you don't want it, use `git rm bar.cxx`.

Finally, git gives us 3 options how to proceed with rebase:
- `git rebase --continue`: accept your manual conflict resolution, move on to next commits
- `git rebase --skip`: git will additionally suggest it if in all conflict you preserved only the HEAD version. The command skips the commit with "Some commit message". For example: you made a PR after 4 commits, then you made further 3 commits. After your PR is merged in the upstream, you rebase on the most recent `upstream/master`. You want git to skip the first 4 commits from your branch history, as they are already included in the merged code at upstream/master.
- `git rebase --abort`: this will restore your branch to the state before the rebase and abort the rebase. Very useful when something gets messed up on the way!
