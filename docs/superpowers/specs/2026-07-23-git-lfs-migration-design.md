# Git LFS 全历史迁移设计

日期：2026-07-23

## 目标

为仓库启用 Git LFS，把 PDF 和常见位图从普通 Git blob 迁移为 LFS
对象，并让后续同类型文件自动进入 LFS。

## 当前状态

- 仓库只有 `main` 和对应的 `origin/main`，没有标签或其他分支。
- 重写前提交为 `15aad307caef19b520fae3ac54e4de4b293c0025`。
- 当前有 4 个 PDF，合计约 3.8 MiB；历史中没有其他超过 100 KiB 的
  二进制文件。
- Git LFS 3.3.0 已安装，远端为 `git@github.com:phateffect/boards.git`。

## 迁移范围

Git LFS 跟踪以下扩展名的小写和大写形式：

- PDF
- PNG
- JPG / JPEG
- GIF
- WebP
- BMP
- TIF / TIFF

SVG 保持普通 Git 文本，以便审阅差异。

## 执行方式

1. 确认工作区干净，并获取最新 `origin/main`。
2. 使用仓库级 Git LFS 初始化，不修改无关仓库。
3. 使用 `git lfs migrate import` 重写全部现有引用和完整历史。
4. 不保留指向旧提交的备份分支或标签，避免普通 PDF blob 继续可达。
5. 验证 `.gitattributes`、当前文件指针、LFS 对象完整性和历史对象。
6. 先上传 LFS 对象，再使用 `git push --force-with-lease origin main`
   替换远端历史。

## 安全约束

- 强制推送只针对 `origin/main`。
- 推送前再次确认远端仍指向迁移开始时记录的提交；若远端发生变化，
  `--force-with-lease` 必须拒绝覆盖。
- 不删除工作树中的 PDF；迁移后工作树仍保留可正常打开的实际内容，
  Git 索引和历史保存 LFS 指针。
- 旧提交 ID 会失效；通过本设计记录保留迁移前的提交 ID 供审计。

## 验证标准

- `git lfs track` 显示所有约定扩展名。
- 4 个现有 PDF 均由 `git lfs ls-files` 列出。
- `git show HEAD:<pdf>` 返回 LFS pointer，而不是 PDF 内容。
- `git lfs fsck` 通过。
- `git status` 在迁移和推送后为空。
- `origin/main` 与重写后的本地 `main` 一致。
