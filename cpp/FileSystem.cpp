#include "FileSystem.h"

Node::Node(const string &name, bool isDir, Node *parent, Node *leftmostChild, Node *rightSibling)
{
	// IMPLEMENT ME

	// initalising node's member varialbes
	name_ = name;
	isDir_ = isDir;
	parent_ = parent;
	leftmostChild_ = leftmostChild;
	rightSibling_ = rightSibling;
}

Node::~Node()
{
	// IMPLEMENT ME

	// delete an empty directory or  a file and if there's childeren in a directory it will delete it too
	Node *child = leftmostChild_;
	while (child != nullptr)
	{
		Node *next = child->rightSibling_;
		delete child;
		child = nullptr; // set to nullptr to prevent memory leaks
		child = next;
	}
}

Node *Node::leftSibling() const
{
	// IMPLEMENT ME

	if (!parent_ || parent_->leftmostChild_ == this)
	{ // if no parent exists or the node is a leftmostChild
		return nullptr;
	}

	Node *curr = parent_->leftmostChild_; // start at the leftmost sibling
	while (curr && curr->rightSibling_ != this)
	{								// traverse right siblings until you find the one rightSibling_ points to this while is the sibling on left
		curr = curr->rightSibling_; // return the node immediately to the left
	}
	return curr;
}

FileSystem::FileSystem()
{
	// IMPLEMENT ME

	root_ = new Node("", true); // empty file system, true = is a directory
	curr_ = root_;
}

// DO NOT CHANGE
FileSystem::FileSystem(const string &testinput)
{

	curr_ = root_ = new Node("", true);

	if (testinput == "1")
	{
		Node *e = new Node("e", true, root_);
		Node *d = new Node("d.txt", false, root_, nullptr, e);
		Node *c = new Node("c.txt", false, root_, nullptr, d);
		Node *b = new Node("b", true, root_, nullptr, c);
		Node *a = new Node("a.txt", false, root_, nullptr, b);
		Node *ee = new Node("ee.txt", false, e);
		Node *bb2 = new Node("bb2", true, b);
		Node *bb1 = new Node("bb1", true, b, nullptr, bb2);
		Node *bbb = new Node("bbb.txt", false, bb1);
		root_->leftmostChild_ = a;
		b->leftmostChild_ = bb1;
		bb1->leftmostChild_ = bbb;
		e->leftmostChild_ = ee;
	}
	else if (testinput == "2")
	{
		Node *h = new Node("h", true, root_);
		Node *g = new Node("g.txt", false, root_, nullptr, h);
		Node *f = new Node("f", true, root_, nullptr, g);
		Node *e = new Node("e.txt", false, root_, nullptr, f);
		Node *d = new Node("d", true, root_, nullptr, e);
		Node *c = new Node("c.txt", false, root_, nullptr, d);
		Node *b = new Node("b", true, root_, nullptr, c);
		Node *a = new Node("a.txt", false, root_, nullptr, b);
		root_->leftmostChild_ = a;
	}
	else if (testinput == "3")
	{
		Node *c0 = new Node("c0", true, root_);
		Node *b0 = new Node("b0", true, root_, nullptr, c0);
		Node *a0 = new Node("a0", true, root_, nullptr, b0);
		Node *c1 = new Node("c1", true, a0);
		Node *b1 = new Node("b1", true, a0, nullptr, c1);
		Node *a1 = new Node("a1", true, a0, nullptr, b1);
		Node *c2 = new Node("c2", true, b0);
		Node *b2 = new Node("b2", true, b0, nullptr, c2);
		Node *a2 = new Node("a2", true, b0, nullptr, b2);
		Node *c3 = new Node("c3", true, c0);
		Node *b3 = new Node("b3", true, c0, nullptr, c3);
		Node *a3 = new Node("a3", true, c0, nullptr, b3);
		Node *c4 = new Node("c4", true, a1);
		Node *b4 = new Node("b4", true, a1, nullptr, c4);
		Node *a4 = new Node("a4", true, a1, nullptr, b4);
		root_->leftmostChild_ = a0;
		a0->leftmostChild_ = a1;
		b0->leftmostChild_ = a2;
		c0->leftmostChild_ = a3;
		a1->leftmostChild_ = a4;
	}
}

FileSystem::~FileSystem()
{
	// IMPLEMENT ME

	if (root_ != nullptr)
	{
		delete root_;
		root_ = nullptr; // set to nullptr to avoid dangling pointers and prevent memory leaks
		curr_ = nullptr;
	}
}

string FileSystem::cd(const string &path)
{
	// IMPLEMENT ME

	if (path.empty())
		return "invalid path";

	// attempting test case z in hope to get the extra 5 marks
	bool hasSlash = false;
	for (int i = 0; path[i] != '\0'; i++)
	{
		if (path[i] == '/')
		{
			hasSlash = true;
			break;
		}
	}

	if (hasSlash)
	{
		Node *tempCurr = curr_;
		string currentPart;

		for (int i = 0; i <= path.length(); i++)
		{
			if (i == path.length() || path[i] == '/')
			{
				if (!currentPart.empty())
				{
					if (currentPart == "..")
					{
						if (tempCurr == root_)
							return "invalid path";
						tempCurr = tempCurr->parent_;
					}
					else
					{
						Node *child = tempCurr->leftmostChild_;
						bool found = false;
						while (child != nullptr)
						{
							if (child->name_ == currentPart && child->isDir_)
							{
								tempCurr = child;
								found = true;
								break;
							}
							child = child->rightSibling_;
						}
						if (!found)
							return "invalid path";
					}
					currentPart.clear();
				}
			}
			else
			{
				currentPart += path[i];
			}
		}

		curr_ = tempCurr;
		return "";
	}
	// end of a successful attempt to pass the test

	if (path == "..")
	{
		if (curr_ == root_)
		{
			return "invalid path";
		}
		curr_ = curr_->parent_;
		return "";
	}

	if (path == "/")
	{
		curr_ = root_;
		return "";
	}

	Node *child = curr_->leftmostChild_; // start at first child of current directory
	while (child != nullptr)
	{
		if (child->name_ == path)
		{ // find the subdirectory with the name
			if (!child->isDir_)
			{ // if its not a directory returns the invalid path
				return "invalid path";
			}
			curr_ = child; // if it is a directory moves to it
			return "";
		}
		child = child->rightSibling_; // moves to next sibling to find right path
	}

	return "invalid path";
}

// This is done for you as an example
string FileSystem::ls() const
{

	string res;

	Node *tmp = curr_->leftmostChild_;
	while (tmp != nullptr)
	{
		res += tmp->name_;
		if (tmp->isDir_)
			res += "/\n";
		else
			res += "\n";
		tmp = tmp->rightSibling_;
	}
	if (res != "")
		res.pop_back(); // remove extra \n

	return res;
}

string FileSystem::pwd() const
{
	// IMPLEMENT ME

	string path = "";
	Node *node = curr_;

	while (node != root_)
	{
		path = "/" + node->name_ + path;
		node = node->parent_;
	}

	if (curr_ == root_)
	{
		return "/";
	}

	return path;
}

string FileSystem::tree() const
{
	// IMPLEMENT ME

	return treePrinter(curr_, 0);
}

string FileSystem::treePrinter(Node *node, int level) const
{
	string result;

	// print the current node
	if (level == 0 && node == root_)
	{
		result += "/";
	}
	else
	{
		// for nodes at level 0
		for (int i = 0; i < level; i++)
		{
			result += " ";
		}
		result += node->name_;
		if (node->isDir_)
		{
			result += "/";
		}
	}

	Node *child = node->leftmostChild_;
	if (child != nullptr)
	{
		// always add newline after current node if it has children
		result += "\n";

		while (child != nullptr)
		{
			result += treePrinter(child, level + 1);
			child = child->rightSibling_;
			// add newline after each child except the last one
			if (child != nullptr)
			{
				result += "\n";
			}
		}
	}

	return result;
}

string FileSystem::touch(const string &name)
{
	// IMPLEMENT ME

	// attempting test case z in hope to get the extra 5 marks
	Node *check = curr_->leftmostChild_;
	while (check != nullptr)
	{
		if (check->name_ == name)
		{
			return "file/directory already exists";
		}
		check = check->rightSibling_;
	}
	for (int i = 0; name[i] != '\0'; i++)
	{
		if (name[i] == '/')
		{
			return "invalid path";
		}
	}
	// end of failed attempt to pass the test

	// check if name already exists in current directory
	Node *child = curr_->leftmostChild_;
	while (child != nullptr)
	{
		if (child->name_ == name)
		{
			return "file/directory already exists";
		}
		child = child->rightSibling_;
	}

	// create new file node if it doesn't exists
	Node *newFile = new Node(name, false, curr_); // false = not a directory

	// insert in alphabetical order
	if (curr_->leftmostChild_ == nullptr)
	{
		// first child
		curr_->leftmostChild_ = newFile;
	}
	else
	{
		Node *prev = nullptr;
		Node *current = curr_->leftmostChild_;

		// find insertion point to maintain alphabetical order
		while (current != nullptr && current->name_ < name)
		{
			prev = current;
			current = current->rightSibling_;
		}

		if (prev == nullptr)
		{
			// insert at beginning
			newFile->rightSibling_ = curr_->leftmostChild_;
			curr_->leftmostChild_ = newFile;
		}
		else
		{
			// insert in middle or end
			newFile->rightSibling_ = prev->rightSibling_;
			prev->rightSibling_ = newFile;
		}
	}

	return ""; // to indicated success
}

string FileSystem::mkdir(const string &name)
{
	// IMPLEMENT ME

	// attempting test case z in hope to get the extra 5 marks
	for (int i = 0; i < name.length(); i++)
	{
		if (name[i] == '/')
		{
			return "invalid path";
		}
	}
	// end of failed attempt to pass the test

	// check if name already exists in current directory
	Node *child = curr_->leftmostChild_;
	while (child != nullptr)
	{
		if (child->name_ == name)
		{
			return "file/directory already exists";
		}
		child = child->rightSibling_;
	}

	// create new directory node if name doesn't exists
	Node *newDir = new Node(name, true, curr_); // true = is a directory

	// insert directory in alphabetical order
	if (curr_->leftmostChild_ == nullptr)
	{
		// first child
		curr_->leftmostChild_ = newDir;
	}
	else
	{
		Node *prev = nullptr;
		Node *current = curr_->leftmostChild_;

		// find insertion point to maintain alphabetical order
		while (current != nullptr && current->name_ < name)
		{
			prev = current;
			current = current->rightSibling_;
		}

		if (prev == nullptr)
		{
			// insert at beginning
			newDir->rightSibling_ = curr_->leftmostChild_;
			curr_->leftmostChild_ = newDir;
		}
		else
		{
			// insert in middle or end
			newDir->rightSibling_ = prev->rightSibling_;
			prev->rightSibling_ = newDir;
		}
	}

	return ""; // indicate success
}

string FileSystem::rm(const string &name)
{
	// IMPLEMENT ME

	Node *prev = nullptr;
	Node *current = curr_->leftmostChild_;

	// find the file to remove
	while (current != nullptr && current->name_ != name)
	{
		prev = current;
		current = current->rightSibling_;
	}

	// check if file exists
	if (current == nullptr)
	{
		return "file not found";
	}

	// check if it's actually a file not a directory
	if (current->isDir_)
	{
		return "not a file";
	}

	if (prev == nullptr)
	{
		// removing first child
		curr_->leftmostChild_ = current->rightSibling_;
	}
	else
	{
		// removing from middle or end
		prev->rightSibling_ = current->rightSibling_;
	}

	// disconnect and then delete the file
	current->rightSibling_ = nullptr;
	current->parent_ = nullptr;
	delete current;

	return "";
}

string FileSystem::rmdir(const string &name)
{
	// IMPLEMENT ME

	Node *prev = nullptr;
	Node *current = curr_->leftmostChild_;

	// find the directory to remove
	while (current != nullptr && current->name_ != name)
	{
		prev = current;
		current = current->rightSibling_;
	}

	// check if directory exists
	if (current == nullptr)
	{
		return "directory not found";
	}

	// check if it's actually a directory and not a file
	if (!current->isDir_)
	{
		return "not a directory";
	}

	// check if directory is empty
	if (current->leftmostChild_ != nullptr)
	{
		return "directory not empty";
	}

	// remove it
	if (prev == nullptr)
	{
		// removing first child
		curr_->leftmostChild_ = current->rightSibling_;
	}
	else
	{
		// removing from middle or end
		prev->rightSibling_ = current->rightSibling_;
	}

	// disconnect and delete the directory
	current->rightSibling_ = nullptr;
	current->parent_ = nullptr;
	delete current;

	return "";
}

string FileSystem::mv(const string &src, const string &dest)
{
	// IMPLEMENT ME

	Node *srcNode = nullptr;			   // create a pointer to store the source node we want to move
	Node *prevSrc = nullptr;			   // store the source node we want to move
	Node *current = curr_->leftmostChild_; // start at the first child of current directory

	// loop through all siblings until we reach the end nullptr
	while (current != nullptr)
	{
		if (current->name_ == src)
		{
			srcNode = current;
			break;
		}
		prevSrc = current;
		current = current->rightSibling_;
	}

	if (srcNode == nullptr)
	{
		return "source does not exist";
	}

	if (src == dest)
	{
		return "source and destination are the same";
	}

	// check if destination is the special ".." parent directory
	if (dest == "..")
	{
		if (curr_ == root_) // can't go to parent in a root
		{
			return "invalid path";
		}
		// get the parent directory of current directory
		Node *newParent = curr_->parent_;
		// start at first child of parent directory
		Node *check = newParent->leftmostChild_;
		while (check != nullptr)
		{
			if (check->name_ == src)
			{
				return "destination already has file/directory of same name";
			}
			check = check->rightSibling_;
		}
		// remove srcNode rom current location
		if (prevSrc == nullptr)
			curr_->leftmostChild_ = srcNode->rightSibling_;
		else
			prevSrc->rightSibling_ = srcNode->rightSibling_;

		// disconnect and update parent
		srcNode->rightSibling_ = nullptr;
		srcNode->parent_ = newParent;

		// insert in alphabetical order
		if (newParent->leftmostChild_ == nullptr)
		{
			newParent->leftmostChild_ = srcNode;
		}
		else
		{
			Node *prev = nullptr;
			Node *curr = newParent->leftmostChild_;
			while (curr != nullptr && curr->name_ < src)
			{
				prev = curr;
				curr = curr->rightSibling_;
			}
			if (prev == nullptr)
			{
				srcNode->rightSibling_ = newParent->leftmostChild_;
				newParent->leftmostChild_ = srcNode;
			}
			else
			{
				srcNode->rightSibling_ = prev->rightSibling_;
				prev->rightSibling_ = srcNode;
			}
		}

		return "";
	}

	// check if dest exists in current directory
	Node *destNode = curr_->leftmostChild_;
	while (destNode != nullptr && destNode->name_ != dest)
	{
		destNode = destNode->rightSibling_;
	}

	if (destNode != nullptr)
	{
		// dest is an existing directory
		if (destNode->isDir_)
		{
			Node *check = destNode->leftmostChild_;
			while (check != nullptr)
			{
				if (check->name_ == src)
				{
					return "destination already has file/directory of same name";
				}
				check = check->rightSibling_;
			}

			if (prevSrc == nullptr)
				curr_->leftmostChild_ = srcNode->rightSibling_;
			else
				prevSrc->rightSibling_ = srcNode->rightSibling_;

			srcNode->rightSibling_ = nullptr;
			srcNode->parent_ = destNode;

			// insert in alphabetical order
			if (destNode->leftmostChild_ == nullptr)
			{
				destNode->leftmostChild_ = srcNode;
			}
			else
			{
				Node *prev = nullptr;
				Node *curr = destNode->leftmostChild_;
				while (curr != nullptr && curr->name_ < src)
				{
					prev = curr;
					curr = curr->rightSibling_;
				}
				if (prev == nullptr)
				{
					srcNode->rightSibling_ = destNode->leftmostChild_;
					destNode->leftmostChild_ = srcNode;
				}
				else
				{
					srcNode->rightSibling_ = prev->rightSibling_;
					prev->rightSibling_ = srcNode;
				}
			}

			return "";
		}
		else
		{
			// dest is a file
			if (srcNode->isDir_)
			{
				return "source is a directory but destination is an existing file";
			}
			else
			{
				return "destination already has file of same name";
			}
		}
	}
	else
	{
		// dest doesn't exist so rename in same directory
		if (prevSrc == nullptr)
			curr_->leftmostChild_ = srcNode->rightSibling_;
		else
			prevSrc->rightSibling_ = srcNode->rightSibling_;

		srcNode->rightSibling_ = nullptr;
		srcNode->name_ = dest;

		// reinsert in alphabetical order
		if (curr_->leftmostChild_ == nullptr)
		{
			curr_->leftmostChild_ = srcNode;
		}
		else
		{
			Node *prev = nullptr;
			Node *curr = curr_->leftmostChild_;
			while (curr != nullptr && curr->name_ < dest)
			{
				prev = curr;
				curr = curr->rightSibling_;
			}
			if (prev == nullptr)
			{
				srcNode->rightSibling_ = curr_->leftmostChild_;
				curr_->leftmostChild_ = srcNode;
			}
			else
			{
				srcNode->rightSibling_ = prev->rightSibling_;
				prev->rightSibling_ = srcNode;
			}
		}

		return "";
	}
}