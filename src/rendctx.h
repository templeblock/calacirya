#ifndef CALA_RENDCTX_H_
#define CALA_RENDCTX_H_

// global overrides to disable features if needed
enum {
	ROPT_MBLUR	= 1,	// motion blur
	ROPT_DOF	= 2		// depth of field
};
#define ROPT_DEFAULT	(ROPT_MBLUR | ROPT_DOF)

struct RenderOptions {
	int width, height;		// output image dimensions
	int samples;			// # samples per pixel
	long tmstart, tmend;	// rendering time interval (animation range)

	unsigned int flags;		// bitmask, see above...

	int blksize;			// render block size

	RenderOptions();		// initialize defaults

	void enable(unsigned int bit);
	void disable(unsigned int bit);
	bool is_enabled(unsigned int bit);
};


class Scene;
class Pixmap;
struct FrameBlock;

class RenderContext {
public:
	RenderOptions opt;
	Scene *scn;
	Pixmap *framebuf;

	FrameBlock *blocks;
	int num_blocks;

	RenderContext();
	~RenderContext();

	bool load_config(const char *fname);
	bool parse_args(int argc, char **argv);

	void set_block_size(int xsz, int ysz);
};

struct FrameBlock {
	int x, y, width, height;
	int scansize;	// full scanline size in pixels
	int prio;		// render priority

	FrameBlock();
	FrameBlock(int x, int y, int xsz, int ysz);
};

#endif	// CALA_RENDCTX_H_
