if tup.getconfig("NO_FASM") ~= "" then return end
ROOT = "../../../.."
tup.rule("ps2mouse.asm", "fasm %f %o " .. tup.getconfig("PESTRIP_CMD") .. tup.getconfig("KPACK_CMD"), "%B.sys")
