#include <assert.h>
#include <algorithm>
#include "xform_node.h"
#include "anim/anim.h"
#include "anim/track.h"

static inline anm_interpolator track_interpolator(Interp in);
static inline anm_extrapolator track_extrapolator(Extrap ex);

XFormNode::XFormNode()
{
	anm = new anm_node;
	anm_init_node(anm);
}

XFormNode::~XFormNode()
{
	anm_destroy_node(anm);
	delete anm;
}

void XFormNode::set_name(const char *name)
{
	anm_set_node_name(anm, name);
}

const char *XFormNode::get_name() const
{
	return anm_get_node_name(anm);
}

void XFormNode::set_interpolator(Interp in)
{
	anm_set_interpolator(anm, track_interpolator(in));
	interp = in;
}

Interp XFormNode::get_interpolator() const
{
	return interp;
}

void XFormNode::set_extrapolator(Extrap ex)
{
	anm_set_extrapolator(anm, track_extrapolator(ex));
	extrap = ex;
}

Extrap XFormNode::get_extrapolator() const
{
	return extrap;
}

void XFormNode::add_child(XFormNode *child)
{
	children.push_back(child);
	anm_link_node(anm, child->anm);
}

void XFormNode::remove_child(XFormNode *child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if(it != children.end()) {
		children.erase(it);
		anm_unlink_node(anm, child->anm);
	}
}

void XFormNode::set_position(const Vector3 &pos, long tmsec)
{
	anm_set_position(anm, v3_cons(pos.x, pos.y, pos.z), ANM_MSEC2TM(tmsec));
}

Vector3 XFormNode::get_node_position(long tmsec) const
{
	vec3_t p = anm_get_node_position(anm, ANM_MSEC2TM(tmsec));
	return Vector3(p.x, p.y, p.z);
}

void XFormNode::set_rotation(const Quaternion &quat, long tmsec)
{
	anm_set_rotation(anm, quat_cons(quat.s, quat.v.x, quat.v.y, quat.v.z), ANM_MSEC2TM(tmsec));
}

Quaternion XFormNode::get_node_rotation(long tmsec) const
{
	quat_t q = anm_get_node_rotation(anm, ANM_MSEC2TM(tmsec));
	return Quaternion(q.w, q.x, q.y, q.z);
}

void XFormNode::set_scaling(const Vector3 &pos, long tmsec)
{
	anm_set_scaling(anm, v3_cons(pos.x, pos.y, pos.z), ANM_MSEC2TM(tmsec));
}

Vector3 XFormNode::get_node_scaling(long tmsec) const
{
	vec3_t s = anm_get_node_scaling(anm, ANM_MSEC2TM(tmsec));
	return Vector3(s.x, s.y, s.z);
}

// these take hierarchy into account
Vector3 XFormNode::get_position(long tmsec) const
{
	vec3_t v = anm_get_position(anm, ANM_MSEC2TM(tmsec));
	return Vector3(v.x, v.y, v.z);
}

Quaternion XFormNode::get_rotation(long tmsec) const
{
	quat_t q = anm_get_rotation(anm, tmsec);
	return Quaternion(q.w, q.x, q.y, q.z);
}

Vector3 XFormNode::get_scaling(long tmsec) const
{
	vec3_t v = anm_get_scaling(anm, ANM_MSEC2TM(tmsec));
	return Vector3(v.x, v.y, v.z);
}

void XFormNode::set_pivot(const Vector3 &pivot)
{
	anm_set_pivot(anm, v3_cons(pivot.x, pivot.y, pivot.z));
}

Vector3 XFormNode::get_pivot() const
{
	vec3_t p = anm_get_pivot(anm);
	return Vector3(p.x, p.y, p.z);
}

void XFormNode::get_xform(long tmsec, Matrix4x4 *mat, Matrix4x4 *inv_mat) const
{
	if(mat) {
		anm_get_matrix(anm, (scalar_t(*)[4])mat, ANM_MSEC2TM(tmsec));
	}
	if(inv_mat) {
		anm_get_inv_matrix(anm, (scalar_t(*)[4])inv_mat, ANM_MSEC2TM(tmsec));
	}
}


// ---- Track ----

Track::Track()
{
	trk = new anm_track;
	anm_init_track(trk);
}

Track::~Track()
{
	anm_destroy_track(trk);
	delete trk;
}

Track::Track(const Track &rhs)
{
	trk = new anm_track;
	anm_init_track(trk);
	anm_copy_track(trk, rhs.trk);
	interp = rhs.interp;
	extrap = rhs.extrap;
}

Track &Track::operator =(const Track &rhs)
{
	if(&rhs == this) {
		return *this;
	}

	anm_copy_track(trk, rhs.trk);
	interp = rhs.interp;
	extrap = rhs.extrap;
	return *this;
}


void Track::set_interpolator(Interp in)
{
	anm_set_track_interpolator(trk, track_interpolator(in));
	interp = in;
}

Interp Track::get_interpolator() const
{
	return interp;
}

void Track::set_extrapolator(Extrap ex)
{
	anm_set_track_extrapolator(trk, track_extrapolator(ex));
	extrap = ex;
}

Extrap Track::get_extrapolator() const
{
	return extrap;
}

void Track::set_default(double def)
{
	anm_set_track_default(trk, def);
}

void Track::set_value(float val, long tmsec)
{
	anm_set_value(trk, ANM_MSEC2TM(tmsec), val);
}

float Track::get_value(long tmsec) const
{
	return anm_get_value(trk, ANM_MSEC2TM(tmsec));
}

float Track::operator ()(long tmsec) const
{
	return anm_get_value(trk, ANM_MSEC2TM(tmsec));
}


// ---- Track3 ----

void Track3::set_interpolator(Interp in)
{
	for(auto trk : track) {
		trk.set_interpolator(in);
	}
}

Interp Track3::get_interpolator() const
{
	return track[0].get_interpolator();
}

void Track3::set_extrapolator(Extrap ex)
{
	for(auto trk : track) {
		trk.set_extrapolator(ex);
	}
}

Extrap Track3::get_extrapolator() const
{
	return track[0].get_extrapolator();
}

void Track3::set_default(const Vector3 &def)
{
	for(int i=0; i<3; i++) {
		track[i].set_default(def[i]);
	}
}

void Track3::set_value(const Vector3 &val, long tmsec)
{
	for(int i=0; i<3; i++) {
		track[i].set_value(val[i], tmsec);
	}
}

Vector3 Track3::get_value(long tmsec) const
{
	return Vector3(track[0](tmsec), track[1](tmsec), track[2](tmsec));
}

Vector3 Track3::operator ()(long tmsec) const
{
	return Vector3(track[0](tmsec), track[1](tmsec), track[2](tmsec));
}


static inline anm_interpolator track_interpolator(Interp in)
{
	switch(in) {
	case Interp::step:
		return ANM_INTERP_STEP;
	case Interp::linear:
		return ANM_INTERP_LINEAR;
	case Interp::cubic:
		return ANM_INTERP_CUBIC;
	}

	assert(0);
	return ANM_INTERP_STEP;
}

static inline anm_extrapolator track_extrapolator(Extrap ex)
{
	switch(ex) {
	case Extrap::extend:
		return ANM_EXTRAP_EXTEND;
	case Extrap::clamp:
		return ANM_EXTRAP_CLAMP;
	case Extrap::repeat:
		return ANM_EXTRAP_REPEAT;
	}

	assert(0);
	return ANM_EXTRAP_EXTEND;
}

