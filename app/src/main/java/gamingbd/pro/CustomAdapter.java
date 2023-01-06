package gamingbd.pro;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class CustomAdapter extends BaseAdapter {


    Context     context;
    Drawable[]  applicationIcons;
    String[]    applicationNames;
    String[]    applicationsPackages;

    LayoutInflater layoutInflater;

    CustomAdapter(Context context, Drawable[]  applicationIcons, String[] applicationNames, String[] applicationsPackages){
        this.context                = context;
        this.applicationIcons       = applicationIcons;
        this.applicationNames       = applicationNames;
        this.applicationsPackages   = applicationsPackages;
    }

    @Override
    public int getCount() {
        return applicationNames.length;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        if ( convertView == null ){
            layoutInflater  = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView     = layoutInflater.inflate(R.layout.sample_view, parent, false);
        }

        ImageView applicationIconImageView      = convertView.findViewById(R.id.applicationIcon);
        TextView  applicationNameTextView       = convertView.findViewById(R.id.applicationName);
        TextView  applicationPackageTextView    = convertView.findViewById(R.id.applicationPackage);

        applicationIconImageView.setImageDrawable(applicationIcons[position]);
        applicationNameTextView.setText(applicationNames[position]);
        applicationPackageTextView.setText(applicationsPackages[position]);

        return convertView;
    }
}
