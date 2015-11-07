package edu.gvsu.cis;

import org.glassfish.jersey.server.ResourceConfig;

import javax.ws.rs.ApplicationPath;
import javax.ws.rs.core.Application;
import java.util.HashSet;
import java.util.Set;


/**
 * Created by dulimarh on 11/7/15.
 */
@ApplicationPath("webapi")
public class PassageWayApplication extends Application {
    @Override
    public Set<Class<?>> getClasses() {
        Set<Class<?>> pset = new HashSet<Class<?>>();
        pset.add(MyResource.class);
        return pset;
    }
}
