char* gitsh_cmds_arr[] = {"add", "checkout", "difftool", "imap-send", "notes",
                      "reset", "submodule", "am", "cherry", "fetch", "init",
                      "pull", "revert", "subtree", "annotate", "cherry-pick",
                      "filter-branch","instaweb", "push", "rm", "tag", "apply",
                      "clean", "format-patch", "interpret-trailers", "rebase",
                      "shortlog", "verify-commit", "archive", "clone", "fsck",
                      "log", "reflog", "show", "whatchanged", "bisect",
                      "commit", "gc", "merge", "remote", "show-branch",
                      "worktree", "blame", "config", "get-tar-commit-id",
                      "mergetool", "repack", "stage", "branch", "describe",
                      "grep", "mv", "replace", "stash", "bundle", "diff",
                      "help", "name-rev", "request-pull", "status", "exit",
                      NULL
                      };

char* gitsh_remote_arr[] = {"add", "get-url", "prune", "remove", "rename",
                            "set-branches", "set-head", "set-url", "show update"
                           };

char* gitsh_configs_arr[] = {"add.ignoreErrors", "core.bigFileThreshold",
"gc.logExpiry", "mergetool.keepBackup", "advice.amWorkDir", "core.checkStat",
"gc.packrefs", "mergetool.keepTemporaries", "advice.commitBeforeMerge",
"core.commentChar", "gc.pruneexpire", "mergetool.prompt", "advice.detachedHead",
"core.compression", "gc.reflogexpire", "merge.verbosity", "advice.ignoredHook",
"core.createObject", "gc.reflogexpireunreachable", "notes.displayRef",
"advice.implicitIdentity", "core.deltaBaseCacheLimit", "gc.rerereresolved",
"notes.rewrite.", "advice.pushAlreadyExists", "core.editor",
"gc.rerereunresolved", "notes.rewrite.amend", "advice.pushFetchFirst",
"core.eol", "gc.worktreePruneExpire", "notes.rewriteMode",
"advice.pushNeedsForce", "core.excludesfile", "gitcvs.allbinary",
"notes.rewrite.rebase", "advice.pushNonFFCurrent", "core.fileMode",
"gitcvs.commitmsgannotation", "notes.rewriteRef", "advice.pushNonFFMatching",
"core.fsyncobjectfiles", "gitcvs.dbdriver", "pack.compression",
"advice.pushUpdateRejected", "core.gitProxy", "gitcvs.dbname",
"pack.deltaCacheLimit", "advice.resolveConflict", "core.hideDotFiles",
"gitcvs.dbpass", "pack.deltaCacheSize", "advice.rmHints", "core.hooksPath",
"gitcvs.dbTableNamePrefix", "pack.depth", "advice.statusHints",
"core.ignorecase", "gitcvs.dbuser", "pack.indexVersion", "advice.statusUoption",
"core.ignoreStat", "gitcvs.enabled", "pack.packSizeLimit", "alias.",
"core.logAllRefUpdates", "gitcvs.logfile", "pack.threads", "am.keepcr",
"core.loosecompression", "gitcvs.usecrlfattr", "pack.window", "am.threeWay",
"core.notesRef", "gui.blamehistoryctx", "pack.windowMemory",
"apply.ignorewhitespace", "core.packedGitLimit", "gui.commitmsgwidth", "pager.",
"apply.whitespace", "core.packedGitWindowSize", "gui.copyblamethreshold",
"pretty.", "branch.", "core.packedRefsTimeout", "gui.diffcontext",
"pull.octopus", "branch.autosetupmerge", "core.pager", "gui.encoding",
"pull.twohead", "branch.autosetuprebase", "core.precomposeUnicode",
"gui.fastcopyblame", "push.default", "browser.", "core.preferSymlinkRefs",
"gui.matchtrackingbranch", "push.followTags", "clean.requireForce",
"core.preloadindex", "gui.newbranchtemplate", "rebase.autosquash",
"color.branch", "core.protectHFS", "gui.pruneduringfetch", "rebase.stat",
"color.branch.current", "core.protectNTFS", "gui.spellingdictionary",
"receive.autogc", "color.branch.local", "core.quotepath", "guitool.",
"receive.denyCurrentBranch", "color.branch.plain",
"core.repositoryFormatVersion", "gui.trustmtime", "receive.denyDeleteCurrent",
"color.branch.remote", "core.safecrlf", "help.autocorrect",
"receive.denyDeletes", "color.decorate.branch", "core.sharedRepository",
"help.browser", "receive.denyNonFastForwards", "color.decorate.HEAD",
"core.sparseCheckout", "help.format", "receive.fsckObjects",
"color.decorate.remoteBranch", "core.splitIndex", "http.lowSpeedLimit",
"receive.unpackLimit", "color.decorate.stash", "core.sshCommand",
"http.lowSpeedTime", "receive.updateserverinfo", "color.decorate.tag",
"core.symlinks", "http.maxRequests", "remote.", "color.diff", "core.trustctime",
"http.minSessions", "remote.pushdefault", "color.diff.commit",
"core.untrackedCache", "http.noEPSV", "remotes.", "color.diff.frag",
"core.warnAmbiguousRefs", "http.postBuffer", "repack.usedeltabaseoffset",
"color.diff.func", "core.whitespace", "http.proxy", "rerere.autoupdate",
"color.diff.meta", "core.worktree", "http.sslCAInfo", "rerere.enabled",
"color.diff.new", "credentialCache.ignoreSIGHUP", "http.sslCAPath",
"sendemail.", "color.diff.old", "credential.helper", "http.sslCert",
"sendemail.aliasesfile", "color.diff.plain", "credential.useHttpPath",
"http.sslCertPasswordProtected", "sendemail.aliasfiletype",
"color.diff.whitespace", "credential.username", "http.sslCipherList",
"sendemail.bcc", "color.grep", "diff.algorithm", "http.sslKey", "sendemail.cc",
"color.grep.context", "diff.autorefreshindex", "http.sslVerify",
"sendemail.cccmd", "color.grep.filename", "diff.external", "http.sslVersion",
"sendemail.chainreplyto", "color.grep.function", "diff.ignoreSubmodules",
"http.useragent", "sendemail.confirm", "color.grep.linenumber",
"diff.mnemonicprefix", "i18n.commitEncoding", "sendemail.envelopesender",
"color.grep.match", "diff.noprefix", "i18n.logOutputEncoding", "sendemail.from",
"color.grep.selected", "diff.renameLimit", "imap.authMethod",
"sendemail.identity", "color.grep.separator", "diff.renames", "imap.folder",
"sendemail.multiedit", "color.interactive", "diff.statGraphWidth", "imap.host",
"sendemail.signedoffbycc", "color.interactive.error", "diff.submodule",
"imap.pass", "sendemail.smtpbatchsize", "color.interactive.header",
"diff.suppressBlankEmpty", "imap.port", "sendemail.smtpdomain",
"color.interactive.help", "diff.tool", "imap.preformattedHTML",
"sendemail.smtpencryption", "color.interactive.prompt", "difftool.",
"imap.sslverify", "sendemail.smtppass", "color.pager", "difftool.prompt",
"imap.tunnel", "sendemail.smtprelogindelay", "color.showbranch",
"diff.wordRegex", "imap.user", "sendemail.smtpserver", "color.status",
"fetch.recurseSubmodules", "init.templatedir", "sendemail.smtpserveroption",
"color.status.added", "fetch.unpackLimit", "instaweb.browser",
"sendemail.smtpserverport", "color.status.changed", "format.attach",
"instaweb.httpd", "sendemail.smtpuser", "color.status.header", "format.cc",
"instaweb.local", "sendemail.suppresscc", "color.status.localBranch",
"format.coverLetter", "instaweb.modulepath", "sendemail.suppressfrom",
"color.status.nobranch", "format.from", "instaweb.port", "sendemail.thread",
"color.status.remoteBranch", "format.headers", "interactive.singlekey",
"sendemail.to", "color.status.unmerged", "format.numbered", "log.date",
"sendemail.tocmd", "color.status.untracked", "format.pretty", "log.decorate",
"sendemail.validate", "color.status.updated", "format.signature",
"log.showroot", "showbranch.default", "color.ui", "format.signoff",
"mailmap.file", "status.relativePaths", "commit.cleanup",
"format.subjectprefix", "man.", "status.showUntrackedFiles", "commit.gpgSign",
"format.suffix", "man.viewer", "status.submodulesummary", "commit.status",
"format.thread", "merge.", "submodule.", "commit.template", "format.to",
"merge.conflictstyle", "tar.umask", "commit.verbose", "gc.", "merge.log",
"transfer.unpackLimit", "core.abbrev", "gc.aggressiveDepth",
"merge.renameLimit", "url.", "core.askpass", "gc.aggressiveWindow",
"merge.renormalize", "user.email", "core.attributesfile", "gc.auto",
"merge.stat", "user.name", "core.autocrlf", "gc.autoDetach", "merge.tool",
"user.signingkey", "core.bare", "gc.autopacklimit", "mergetool.", "web.browser",
NULL
};
