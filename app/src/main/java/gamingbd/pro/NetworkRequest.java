package gamingbd.pro;

import android.content.Context;
import android.os.AsyncTask;
import static gamingbd.pro.LoginActivity.*;

/** @brief Handling Non UI Thread for Network Request
 *
 *  To prevent blocking the main UI thread, network request must run in a different thread.
 *  We are creating a class that uses AsyncTask class to implement a non UI background thread.
 *
 *  doInBackground() method calls "doInBackgroundNative" C/C++ function
 *  onPostExecute    method calls "onPostExecuteNative"  C/C++ function
 *
*/

public class NetworkRequest extends AsyncTask<Void, Void, String>  {

    Context context;

    /**@brief Constructor
     *
     * @param context
     */
    public NetworkRequest(Context context) {
        this.context = context;
    }


    @Override
    public String doInBackground(Void... voids) {
        /** Calling C/C++ function that makes network request to server & returns response */
        return doInBackgroundLoginNative();
    }

    @Override
    public void onPostExecute(String response){
        /** Calling C/C++ function that receives the response from server & start post processing*/
       onPostExecuteLoginNative(context, response );
    }
}
