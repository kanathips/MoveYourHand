#ifndef WINDOWS_H
#define WINDOWS_H

class win_cre
{
    public:
        const char *win_name;
        int low_h = 0, low_s = 60, low_v = 69;
        int most_h = 21, most_s = 129, most_v = 144;
        int dil_x = 1, dil_y = 1;
        int erd_x = 1, erd_y = 1;
        win_cre(const char *win_name);
};

#endif // WINDOWS_H
