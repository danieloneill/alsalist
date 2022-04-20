#include <alsa/asoundlib.h>

int main(int argc, char **argv)
{
	snd_seq_t *seq;
	snd_seq_port_info_t *pinfo;
	snd_seq_client_info_t *cinfo;

	int ret = snd_seq_open(&seq, "default", SND_SEQ_OPEN_INPUT, 0);
	if( ret != 0 ) return -1;

	snd_seq_set_client_name(seq, "alsaconnector");

	ret = snd_seq_client_info_malloc(&cinfo);
	if( ret != 0 ) return -2;

	ret = snd_seq_port_info_malloc(&pinfo);
	if( ret != 0 ) return -3;

	snd_seq_client_info_set_client(cinfo, -1);
	while (snd_seq_query_next_client(seq, cinfo) >= 0) {
		int cid = snd_seq_client_info_get_client(cinfo);
		if( cid == snd_seq_client_id( seq ) ) continue; // don't need to look at ourself..

		const char *cname = snd_seq_client_info_get_name(cinfo);
		snd_seq_port_info_set_client(pinfo, cid);
		int nports = snd_seq_client_info_get_num_ports(cinfo);

		snd_seq_port_info_set_port(pinfo, -1);
		while( snd_seq_query_next_port(seq, pinfo) >= 0 )
		{
			int pid = snd_seq_port_info_get_port(pinfo);
			const char *pname = snd_seq_port_info_get_name(pinfo);
			printf("%d:%d %s : %s\n", cid, pid, cname, pname);
		}
	}
	
	snd_seq_port_info_free(pinfo);
	snd_seq_client_info_free(cinfo);
	snd_seq_close( seq );
	return 0;
}
