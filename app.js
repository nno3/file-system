class Node {
    constructor(name, isDir, parent = null, leftmostChild = null, rightSibling = null) {
        this.name = name;
        this.isDir = isDir;
        this.parent = parent;
        this.leftmostChild = leftmostChild;
        this.rightSibling = rightSibling;
    }
}

class FileSystem {
    constructor(testinput) {
        this.root = new Node("", true);
        this.curr = this.root;
        if (testinput === "1") this.loadOne();
        else if (testinput === "2") this.loadTwo();
        else if (testinput === "3") this.loadThree();
    }

    loadOne() {
        const e = new Node("e", true, this.root);
        const d = new Node("d.txt", false, this.root, null, e);
        const c = new Node("c.txt", false, this.root, null, d);
        const b = new Node("b", true, this.root, null, c);
        const a = new Node("a.txt", false, this.root, null, b);
        const ee = new Node("ee.txt", false, e);
        const bb2 = new Node("bb2", true, b);
        const bb1 = new Node("bb1", true, b, null, bb2);
        const bbb = new Node("bbb.txt", false, bb1);
        this.root.leftmostChild = a;
        b.leftmostChild = bb1;
        bb1.leftmostChild = bbb;
        e.leftmostChild = ee;
    }

    loadTwo() {
        const h = new Node("h", true, this.root);
        const g = new Node("g.txt", false, this.root, null, h);
        const f = new Node("f", true, this.root, null, g);
        const e = new Node("e.txt", false, this.root, null, f);
        const d = new Node("d", true, this.root, null, e);
        const c = new Node("c.txt", false, this.root, null, d);
        const b = new Node("b", true, this.root, null, c);
        const a = new Node("a.txt", false, this.root, null, b);
        this.root.leftmostChild = a;
    }

    loadThree() {
        const c0 = new Node("c0", true, this.root);
        const b0 = new Node("b0", true, this.root, null, c0);
        const a0 = new Node("a0", true, this.root, null, b0);
        const c1 = new Node("c1", true, a0);
        const b1 = new Node("b1", true, a0, null, c1);
        const a1 = new Node("a1", true, a0, null, b1);
        const c2 = new Node("c2", true, b0);
        const b2 = new Node("b2", true, b0, null, c2);
        const a2 = new Node("a2", true, b0, null, b2);
        const c3 = new Node("c3", true, c0);
        const b3 = new Node("b3", true, c0, null, c3);
        const a3 = new Node("a3", true, c0, null, b3);
        const c4 = new Node("c4", true, a1);
        const b4 = new Node("b4", true, a1, null, c4);
        const a4 = new Node("a4", true, a1, null, b4);
        this.root.leftmostChild = a0;
        a0.leftmostChild = a1;
        b0.leftmostChild = a2;
        c0.leftmostChild = a3;
        a1.leftmostChild = a4;
    }

    cd(path) {
        if (path === "") return "invalid path";
        if (path.includes("/")) {
            let tempCurr = this.curr;
            let currentPart = "";
            for (let i = 0; i <= path.length; i += 1) {
                if (i === path.length || path[i] === "/") {
                    if (currentPart !== "") {
                        if (currentPart === "..") {
                            if (tempCurr === this.root) return "invalid path";
                            tempCurr = tempCurr.parent;
                        } else {
                            let child = tempCurr.leftmostChild;
                            let found = false;
                            while (child) {
                                if (child.name === currentPart && child.isDir) {
                                    tempCurr = child;
                                    found = true;
                                    break;
                                }
                                child = child.rightSibling;
                            }
                            if (!found) return "invalid path";
                        }
                        currentPart = "";
                    }
                } else {
                    currentPart += path[i];
                }
            }
            this.curr = tempCurr;
            return "";
        }
        if (path === "..") {
            if (this.curr === this.root) return "invalid path";
            this.curr = this.curr.parent;
            return "";
        }
        if (path === "/") {
            this.curr = this.root;
            return "";
        }
        let child = this.curr.leftmostChild;
        while (child) {
            if (child.name === path) {
                if (!child.isDir) return "invalid path";
                this.curr = child;
                return "";
            }
            child = child.rightSibling;
        }
        return "invalid path";
    }

    ls() {
        let result = "";
        let node = this.curr.leftmostChild;
        while (node) {
            result += node.name + (node.isDir ? "/\n" : "\n");
            node = node.rightSibling;
        }
        if (result !== "") result = result.slice(0, -1);
        return result;
    }

    pwd() {
        if (this.curr === this.root) return "/";
        let path = "";
        let node = this.curr;
        while (node !== this.root) {
            path = `/${node.name}${path}`;
            node = node.parent;
        }
        return path;
    }

    tree() {
        return this.treePrinter(this.curr, 0);
    }

    treePrinter(node, level) {
        let result = "";
        if (level === 0 && node === this.root) result += "/";
        else {
            result += " ".repeat(level) + node.name;
            if (node.isDir) result += "/";
        }
        let child = node.leftmostChild;
        if (child) {
            result += "\n";
            while (child) {
                result += this.treePrinter(child, level + 1);
                child = child.rightSibling;
                if (child) result += "\n";
            }
        }
        return result;
    }

    insertAlphabetically(parent, node, name) {
        if (!parent.leftmostChild) {
            parent.leftmostChild = node;
            return;
        }
        let prev = null;
        let current = parent.leftmostChild;
        while (current && current.name < name) {
            prev = current;
            current = current.rightSibling;
        }
        if (!prev) {
            node.rightSibling = parent.leftmostChild;
            parent.leftmostChild = node;
        } else {
            node.rightSibling = prev.rightSibling;
            prev.rightSibling = node;
        }
    }

    nameExists(name) {
        let child = this.curr.leftmostChild;
        while (child) {
            if (child.name === name) return true;
            child = child.rightSibling;
        }
        return false;
    }

    touch(name) {
        if (this.nameExists(name)) return "file/directory already exists";
        if (name.includes("/")) return "invalid path";
        const file = new Node(name, false, this.curr);
        this.insertAlphabetically(this.curr, file, name);
        return "";
    }

    mkdir(name) {
        if (name.includes("/")) return "invalid path";
        if (this.nameExists(name)) return "file/directory already exists";
        const dir = new Node(name, true, this.curr);
        this.insertAlphabetically(this.curr, dir, name);
        return "";
    }

    unlink(prev, current) {
        if (!prev) this.curr.leftmostChild = current.rightSibling;
        else prev.rightSibling = current.rightSibling;
        current.rightSibling = null;
        current.parent = null;
    }

    rm(name) {
        let prev = null;
        let current = this.curr.leftmostChild;
        while (current && current.name !== name) {
            prev = current;
            current = current.rightSibling;
        }
        if (!current) return "file not found";
        if (current.isDir) return "not a file";
        this.unlink(prev, current);
        return "";
    }

    rmdir(name) {
        let prev = null;
        let current = this.curr.leftmostChild;
        while (current && current.name !== name) {
            prev = current;
            current = current.rightSibling;
        }
        if (!current) return "directory not found";
        if (!current.isDir) return "not a directory";
        if (current.leftmostChild) return "directory not empty";
        this.unlink(prev, current);
        return "";
    }

    mv(src, dest) {
        let srcNode = null;
        let prevSrc = null;
        let current = this.curr.leftmostChild;
        while (current) {
            if (current.name === src) {
                srcNode = current;
                break;
            }
            prevSrc = current;
            current = current.rightSibling;
        }
        if (!srcNode) return "source does not exist";
        if (src === dest) return "source and destination are the same";
        if (dest === "..") {
            if (this.curr === this.root) return "invalid path";
            const newParent = this.curr.parent;
            let check = newParent.leftmostChild;
            while (check) {
                if (check.name === src) return "destination already has file/directory of same name";
                check = check.rightSibling;
            }
            this.unlink(prevSrc, srcNode);
            srcNode.parent = newParent;
            this.insertAlphabetically(newParent, srcNode, src);
            return "";
        }
        let destNode = this.curr.leftmostChild;
        while (destNode && destNode.name !== dest) destNode = destNode.rightSibling;
        if (destNode) {
            if (destNode.isDir) {
                let check = destNode.leftmostChild;
                while (check) {
                    if (check.name === src) return "destination already has file/directory of same name";
                    check = check.rightSibling;
                }
                this.unlink(prevSrc, srcNode);
                srcNode.parent = destNode;
                this.insertAlphabetically(destNode, srcNode, src);
                return "";
            }
            if (srcNode.isDir) return "source is a directory but destination is an existing file";
            return "destination already has file of same name";
        }
        this.unlink(prevSrc, srcNode);
        srcNode.parent = this.curr;
        srcNode.name = dest;
        this.insertAlphabetically(this.curr, srcNode, dest);
        return "";
    }
}

let fs = new FileSystem();
const log = document.getElementById("log");
const prompt = document.getElementById("prompt");
const form = document.getElementById("line");
const input = document.getElementById("command");

function showPrompt() {
    prompt.textContent = `${fs.pwd()}> `;
}

function write(text) {
    log.textContent += text;
    log.parentElement.scrollTop = log.parentElement.scrollHeight;
}

function run(line) {
    const parts = line.trim().split(/\s+/).filter(Boolean);
    const cmd = parts[0] || "";
    const arg1 = parts[1] || "";
    const arg2 = parts[2] || "";
    write(`${fs.pwd()}> ${line}\n`);
    if (cmd === "exit") {
        write("The page stays open.\n");
        return;
    }
    if (cmd === "load") {
        fs = new FileSystem(arg1);
        showPrompt();
        return;
    }
    let output = "command not found";
    if (cmd === "cd") output = fs.cd(arg1);
    else if (cmd === "ls") output = fs.ls();
    else if (cmd === "pwd") output = fs.pwd();
    else if (cmd === "tree") output = fs.tree();
    else if (cmd === "touch") output = fs.touch(arg1);
    else if (cmd === "mkdir") output = fs.mkdir(arg1);
    else if (cmd === "rm") output = fs.rm(arg1);
    else if (cmd === "rmdir") output = fs.rmdir(arg1);
    else if (cmd === "mv") output = fs.mv(arg1, arg2);
    if (output !== "") write(`${output}\n`);
    showPrompt();
}

form.addEventListener("submit", (event) => {
    event.preventDefault();
    const line = input.value;
    input.value = "";
    run(line);
});

showPrompt();
