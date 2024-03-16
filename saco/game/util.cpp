
#include <windows.h>
#include <stdio.h>
#include "../main.h"
#include <sys/stat.h>

DWORD dwPlayerPedPtrs[PLAYER_PED_SLOTS];

struct struc_96
{
	char _gap0[16];
};
struc_96 unnamed_1026C258[PLAYER_PED_SLOTS];

#define NUM_RADAR_COLORS 1000

DWORD dwHudColors[NUM_RADAR_COLORS] = {
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (100)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (200)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (300)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (400)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (500)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (600)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (700)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (800)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (900)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF
};

DWORD dwUseHudColors[NUM_RADAR_COLORS];

//-----------------------------------------------------------

void  ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1)
{
	_asm mov eax, 0x56BA00
	_asm jmp eax
}

//-----------------------------------------------------------

void __stdcall WorldAddEntity(DWORD *dwEnt)
{
	_asm push dwEnt
	_asm mov ebx, 0x563220
	_asm call ebx
	_asm pop ebx
}

//-----------------------------------------------------------

void __stdcall WorldRemoveEntity(DWORD *dwEnt)
{
	_asm push dwEnt
	_asm mov ebx, 0x563280
	_asm call ebx
	_asm pop ebx
}

//-----------------------------------------------------------

void __stdcall GameDisableCheatCodes()
{

}

//-----------------------------------------------------------

PED_TYPE * __stdcall GamePool_Ped_GetAt(int iID)
{
	PED_TYPE *pActorRet;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_ACTOR_FROM_ID
	_asm call ebx
	_asm mov pActorRet, eax

	return pActorRet;	
}

//-----------------------------------------------------------

int __stdcall GamePool_Ped_GetIndex(PED_TYPE *pActor)
{
	int iRetVal;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push pActor
	_asm mov ebx, ADDR_ID_FROM_ACTOR
	_asm call ebx
	_asm mov iRetVal, eax

	return iRetVal;
}

//-----------------------------------------------------------

VEHICLE_TYPE * __stdcall GamePool_Vehicle_GetAt(int iID)
{
	VEHICLE_TYPE *pVehicleRet;

	_asm mov ebx, ADDR_VEHICLE_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_VEHICLE_FROM_ID
	_asm call ebx
	_asm mov pVehicleRet, eax

	return pVehicleRet;
}

//-----------------------------------------------------------

DWORD __stdcall GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle)
{
	DWORD dwID=0;

	_asm mov eax, ADDR_VEHICLE_TABLE
	_asm mov ecx, [eax]
	_asm push pVehicle
	_asm mov edx, 0x424160
	_asm call edx
	_asm mov dwID, eax

	return dwID;
}

//-----------------------------------------------------------

ENTITY_TYPE * __stdcall GamePool_Object_GetAt(int iID)
{
	ENTITY_TYPE *pObjectRet;

	_asm mov ebx, 0xB7449C
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, 0x465040
	_asm call ebx
	_asm mov pObjectRet, eax

	return pObjectRet;
}

//-----------------------------------------------------------
// Return the PED_TYPE * of the local player actor.

PED_TYPE * __stdcall GamePool_FindPlayerPed()
{
	return *(PED_TYPE **)(0xB7CD98);
}

//-----------------------------------------------------------
// Translate Weapon model ID into actual weapon ID.

int __stdcall GameGetWeaponModelIDFromWeaponID(int iWeaponID)
{
	switch(iWeaponID)
	{
	case WEAPON_BRASSKNUCKLE:
		return WEAPON_MODEL_BRASSKNUCKLE;

	case WEAPON_GOLFCLUB:
		return WEAPON_MODEL_GOLFCLUB;

	case WEAPON_NITESTICK:
		return WEAPON_MODEL_NITESTICK;

	case WEAPON_KNIFE:
		return WEAPON_MODEL_KNIFE;

	case WEAPON_BAT:
		return WEAPON_MODEL_BAT;

	case WEAPON_SHOVEL:
		return WEAPON_MODEL_SHOVEL;

	case WEAPON_POOLSTICK:
		return WEAPON_MODEL_POOLSTICK;

	case WEAPON_KATANA:
		return WEAPON_MODEL_KATANA;

	case WEAPON_CHAINSAW:
		return WEAPON_MODEL_CHAINSAW;

	case WEAPON_DILDO:
		return WEAPON_MODEL_DILDO;

	case WEAPON_DILDO2:
		return WEAPON_MODEL_DILDO2;

	case WEAPON_VIBRATOR:
		return WEAPON_MODEL_VIBRATOR;

	case WEAPON_VIBRATOR2:
		return WEAPON_MODEL_VIBRATOR2;

	case WEAPON_FLOWER:
		return WEAPON_MODEL_FLOWER;

	case WEAPON_CANE:
		return WEAPON_MODEL_CANE;

	case WEAPON_GRENADE:
		return WEAPON_MODEL_GRENADE;

	case WEAPON_TEARGAS:
		return WEAPON_MODEL_TEARGAS;

	case WEAPON_MOLTOV:
		return WEAPON_MODEL_MOLTOV;

	case WEAPON_COLT45:
		return WEAPON_MODEL_COLT45;

	case WEAPON_SILENCED:
		return WEAPON_MODEL_SILENCED;

	case WEAPON_DEAGLE:
		return WEAPON_MODEL_DEAGLE;

	case WEAPON_SHOTGUN:
		return WEAPON_MODEL_SHOTGUN;

	case WEAPON_SAWEDOFF:
		return WEAPON_MODEL_SAWEDOFF;

	case WEAPON_SHOTGSPA:
		return WEAPON_MODEL_SHOTGSPA;

	case WEAPON_UZI:
		return WEAPON_MODEL_UZI;

	case WEAPON_MP5:
		return WEAPON_MODEL_MP5;

	case WEAPON_AK47:
		return WEAPON_MODEL_AK47;

	case WEAPON_M4:
		return WEAPON_MODEL_M4;

	case WEAPON_TEC9:
		return WEAPON_MODEL_TEC9;

	case WEAPON_RIFLE:
		return WEAPON_MODEL_RIFLE;

	case WEAPON_SNIPER:
		return WEAPON_MODEL_SNIPER;

	case WEAPON_ROCKETLAUNCHER:
		return WEAPON_MODEL_ROCKETLAUNCHER;

	case WEAPON_HEATSEEKER:
		return WEAPON_MODEL_HEATSEEKER;

	case WEAPON_FLAMETHROWER:
		return WEAPON_MODEL_FLAMETHROWER;

	case WEAPON_MINIGUN:
		return WEAPON_MODEL_MINIGUN;

	case WEAPON_SATCHEL:
		return WEAPON_MODEL_SATCHEL;

	case WEAPON_BOMB:
		return WEAPON_MODEL_BOMB;

	case WEAPON_SPRAYCAN:
		return WEAPON_MODEL_SPRAYCAN;

	case WEAPON_FIREEXTINGUISHER:
		return WEAPON_MODEL_FIREEXTINGUISHER;

	case WEAPON_CAMERA:
		return WEAPON_MODEL_CAMERA;

	case WEAPON_NIGHTVISION:
		return WEAPON_MODEL_NIGHTVISION;

	case WEAPON_INFRARED:
		return WEAPON_MODEL_INFRARED;

	case WEAPON_PARACHUTE:
		return WEAPON_MODEL_PARACHUTE;

	}

	return -1;
}

//-----------------------------------------------------------

void __stdcall SetRadarColor(int nIndex,DWORD dwColor)
{
	if(nIndex < sizeof(dwUseHudColors)) {
		dwUseHudColors[nIndex] = dwColor;
	}
}

DWORD __stdcall TranslateColorCodeToRGBA(int iCode)
{
	// Special internal colors
	if(iCode == 1004) return 0x89A4C440;
	if(iCode == 1005) return 0xAA0000FF;
	if(iCode == 1006) return 0xE2C063FF;

	if(iCode < sizeof(dwUseHudColors)) {
		return dwUseHudColors[iCode];
	}
}

void GameResetRadarColors()
{
	memcpy(&dwUseHudColors[0],&dwHudColors[0],sizeof(DWORD)*NUM_RADAR_COLORS);
}


//-----------------------------------------------------------

void __stdcall InitPlayerPedPtrRecords() 
{
	memset(&dwPlayerPedPtrs[0],0,sizeof(DWORD) * PLAYER_PED_SLOTS);
	memset(unnamed_1026C258, 0, sizeof(unnamed_1026C258));
}

//-----------------------------------------------------------

void __stdcall SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr) 
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;

	memset(&unnamed_1026C258[bytePlayer], 0, sizeof(struc_96));
}

//-----------------------------------------------------------

DWORD __stdcall GetPlayerPedPtrRecord(BYTE bytePlayer)
{
	return dwPlayerPedPtrs[bytePlayer];
}

//-----------------------------------------------------------

BYTE __stdcall FindPlayerNumFromPedPtr(DWORD dwPedPtr)
{
	BYTE x = 0;
	while(x != PLAYER_PED_SLOTS)
	{
		if(dwPlayerPedPtrs[x] == dwPedPtr) return x;
		x++;
	}
	return 0;
}

//-----------------------------------------------------------






bool unnamed_100B4B50(VECTOR *vecPos)
{
	return
		vecPos->X < 20000.0f && vecPos->X > -20000.0f &&
		vecPos->Y < 20000.0f && vecPos->Y > -20000.0f &&
		vecPos->Z < 100000.0f && vecPos->Z > -10000.0f;
}






//----------------------------------------------------

bool IsNumeric(char * szString)
{
	while(*szString) {
		if(*szString < '0' || *szString > '9') {
			return false;
		}
		szString++;
	}
	return true;
}

//----------------------------------------------------

DWORD CamFrameBuffer=0;
DWORD FrameBufferTexture=0;

extern CChatWindow * pChatWindow;

void _VehicleEnvMappingFromScreen()
{
	/*
	if(FrameBufferTexture) {
		_asm push FrameBufferTexture
		_asm mov edx, 0x7F3820
		_asm call edx
		_asm pop edx
		FrameBufferTexture = 0;
	}*/

	// get the frame buffer from the main RwCamera
	_asm mov eax, 0xC402D8
	_asm mov ebx, [eax]
	_asm mov CamFrameBuffer, ebx

	_asm push CamFrameBuffer
	_asm mov edx, 0x7F37C0 // RwTextureCreate
	_asm call edx
	_asm pop edx
	_asm mov FrameBufferTexture, eax

	if(!FrameBufferTexture) {
		pChatWindow->AddDebugMessage("No texture!");
	}

	*(DWORD *)0xB4E690 = FrameBufferTexture;
	*(DWORD *)0xB4E68C = FrameBufferTexture;
	*(DWORD *)0xB4E47C = FrameBufferTexture;
	*(DWORD *)0xB4E3EC = FrameBufferTexture;
	*(float *)0x8A7780 = 2.0f;
}

//----------------------------------------------------

int Width,Height,Depth;
DWORD dwRwImage;

void WriteRwRasterToFile(DWORD raster,char *filename)
{
	// RwImageCreate(width,height,depth)
	_asm push Depth
	_asm push Height
	_asm push Width
	_asm mov edx, 0x8026E0
	_asm call edx
	_asm mov dwRwImage, eax
	_asm pop edx
	_asm pop edx
	_asm pop edx

	// RwImageAllocatePixels
	_asm push dwRwImage
	_asm mov edx, 0x8027A0
	_asm call edx
	_asm pop edx

	// RwImageSetFromRaster
	_asm push raster
	_asm push dwRwImage
	_asm mov edx, 0x804250
	_asm call edx
	_asm pop edx
	_asm pop edx

	// RtBmpImageWrite
	_asm push filename
	_asm push dwRwImage
	_asm mov edx, 0x7CE990
	_asm call edx
	_asm pop edx
	_asm pop edx

	// RwImageDestroy
	_asm push dwRwImage
	_asm mov edx, 0x802740
	_asm call edx
	_asm pop edx
}

//----------------------------------------------------

BOOL IsFileOrDirectoryExists(char * szPath)
{
	struct _stat buf;

	return _stat(szPath, &buf) == 0;
}

//----------------------------------------------------


DWORD unnamed_100B6100(char *szString, int nMaxLen)
{
	char tmp_buf[2049];
	memset(tmp_buf, 0, sizeof(tmp_buf));

	if(szString &&
		strlen(szString) < 400 &&
		strstr(szString, "~k~") != NULL)
	{
		strcpy(tmp_buf, szString);

		_asm lea edx, tmp_buf
		_asm push edx
		_asm mov edx, 0x69E160
		_asm call edx
		_asm pop edx

		if((int)strlen(tmp_buf) > nMaxLen) return 0;

		strcpy(szString, tmp_buf);
		return strlen(tmp_buf);
	}
	return 0;
}



