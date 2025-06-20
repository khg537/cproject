#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void o1_fetch_value_from_terminal(const char *sys_cmd, char *retVal)
{
    FILE *fp;
    char read_buf[100] = {0};

    fp = popen(sys_cmd, "r");
    if (fp == NULL)
    {
        perror("popen failed");
        return;
    }

    // if (fgets(read_buf, sizeof(read_buf), fp) != NULL) {
    //     read_buf[strcspn(read_buf, "\n")] = '\0';
    //     strcpy(retVal, read_buf);
    // }
    fscanf(fp, "%s", read_buf);
    strcpy(retVal, read_buf);
    pclose(fp);
}

int main()
{
    char val[1024];

    char cmd[1024];
    int num_cell = 0;
    int num_cbsd = 0;
    int num_gnb = 1;
    char fccid[10];
    char serialnum[10];

    memset(cmd, '\0', sizeof cmd);

    sprintf(cmd, "xmllint --xpath 'count(/cbrsInstallationFile/gNB[%d]/cell)' cbrsinstall.xml", num_gnb);
    o1_fetch_value_from_terminal(cmd, val);
    num_cell = atoi(val);
    printf("number of cell is %d\n", num_cell);

    for (int i = 0; i < num_cell; i++)
    {
        snprintf(cmd, sizeof(cmd), "xmllint --xpath 'count(/cbrsInstallationFile/gNB[%d]/cell[%d]/cbsd)' cbrsinstall.xml", num_gnb, i + 1);
        o1_fetch_value_from_terminal(cmd, val);
        num_cbsd = atoi(val);
        printf("%d cell 's number of cbsd is %d\n", i + 1, num_cbsd);

        for (int j = 0; j < num_cbsd; j++)
        {

            snprintf(cmd, sizeof(cmd), "xmllint --xpath '/cbrsInstallationFile/gNB[%d]/cell[%d]/cbsd[%d]/FccId/text()' cbrsinstall.xml", num_gnb, i + 1, j + 1);
            o1_fetch_value_from_terminal(cmd, val);
            printf("%d cell %d cbsd 's FccId is is %s\n", i + 1, j + 1, val);

            snprintf(cmd, sizeof(cmd), "xmllint --xpath '/cbrsInstallationFile/gNB[%d]/cell[%d]/cbsd[%d]/CbsdSerialNumber/text()' cbrsinstall.xml", num_gnb, i + 1, j + 1);
            o1_fetch_value_from_terminal(cmd, val);
            printf("%d cell %d cbsd 's serialnumber is is %s\n", i + 1, j + 1, val);
        }
    }

    snprintf(cmd, sizeof(cmd), "xmllint --xpath '/cbrsInstallationFile/dp_supported/text()' cbrsinstall.xml");
    o1_fetch_value_from_terminal(cmd, val);
    printf("dp_supported is %s\n", val);

    snprintf(cmd, sizeof(cmd), "xmllint --xpath '/cbrsInstallationFile/dp_url/text()' cbrsinstall.xml");
    o1_fetch_value_from_terminal(cmd, val);
    printf("dp_url is is %s\n", val);

    return 0;
}
