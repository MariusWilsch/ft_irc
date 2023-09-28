#Channel management: 
    + Server response:
        * When the client joins the channel the server sends a response to the client, the user receives a JOIN message as
        confirmation and is then sent the channel's topic (using RPL_TOPIC) and the list of users who are on the channel (using RPL_NAMREPLY)
    

# Channel commands: Manage channel modes
    + MODE command with option that affect a specific user:
        * +/- o : Channel operator privilege.
    + MODE command with option that affect the whole channel;
        * +/- i : Invel only channel.
        * -/+ t : the restrictions of the TOPIC command to channel operators.
        * -/+ k : The channel key (password).
        * -/+ l : The user limit to channel.
    
